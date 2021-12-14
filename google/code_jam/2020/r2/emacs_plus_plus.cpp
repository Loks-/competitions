#include "common/data_structures/unsigned_set.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/dijkstra_mask.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/graph/graph_ei/inverse_edges.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"
#include "common/vector/enumerate.h"
#include "common/vector/read.h"
#include "common/vector/sum.h"

#include <functional>
#include <stack>
#include <string>

int main_emacs_plus_plus() {
  unsigned T, K, Q;
  uint64_t max_cost = PowU<uint64_t>(10u, 12);
  graph::EdgeCostProxy<uint64_t> ep;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> K >> Q;
    string s;
    cin >> s;
    auto vl = nvector::Read<uint64_t>(K), vr = nvector::Read<uint64_t>(K),
         vp = nvector::Read<uint64_t>(K);
    vector<unsigned> vj(K), vs = nvector::Read<unsigned>(Q),
                            ve = nvector::Read<unsigned>(Q);
    for (auto& v : vs) v -= 1;
    for (auto& v : ve) v -= 1;
    stack<unsigned> st;
    for (unsigned i = 0; i < K; ++i) {
      if (s[i] == '(') {
        st.push(i);
      } else {
        assert(!st.empty());
        vj[i] = st.top();
        vj[st.top()] = i;
        st.pop();
      }
    }
    assert(st.empty());
    DirectedGraphEI<uint64_t> g(K);
    for (unsigned i = 0; i < K; ++i) {
      if (i > 0) g.AddEdge(i, i - 1, vl[i]);
      if (i < (K - 1)) g.AddEdge(i, i + 1, vr[i]);
      g.AddEdge(i, vj[i], vp[i]);
    }
    auto ginv = InverseEdges(g);

    vector<uint64_t> vbest(Q, max_cost);
    auto vmapk = nvector::Enumerate<unsigned>(0, K);
    auto vmapq = nvector::Enumerate<unsigned>(0, Q);
    ds::UnsignedSet mask(K);
    vector<unsigned> vtm0k(K), vtm1k(K), vtm2k(K), vtm0q(Q), vtm1q(Q), vtm2q(Q);
    vector<uint64_t> vtd0(K), vtd0f(K), vtd0i(K), vtd1f(K), vtd1i(K);

    std::function<void(unsigned, unsigned, unsigned, unsigned, bool)> Solve =
        [&](unsigned kb, unsigned ke, unsigned qb, unsigned qe,
            bool stl) -> void {
      if (qb >= qe) return;
      mask.Clear();
      for (unsigned k = kb; k < ke; ++k) mask.Insert(vmapk[k]);
      // if (ke - kb <= 4) {
      if (qe - qb <= 4) {
        // Greedy solution
        for (unsigned q = qb; q < qe; ++q) {
          unsigned qm = vmapq[q];
          graph::distance::DijkstraMask(g, ep, vs[qm], max_cost, mask, vtd0);
          vbest[qm] = min(vbest[qm], vtd0[ve[qm]]);
        }
      } else {
        // Find split
        auto ks = kb;
        if (stl) {
          unsigned bestl = ke - kb;
          for (unsigned k = kb, l; k < ke; k = l) {
            unsigned kj = vj[vmapk[k]];
            for (l = k + 1; l < ke; ++l) {
              if (vmapk[l] == kj) break;
            }
            if (l++ == ke) break;
            unsigned score = max(max(k - kb, l - k - 2), ke - l);
            if (score < bestl) {
              bestl = score;
              ks = k;
            }
          }
        } else {
          unsigned bestl = ke - kb;
          for (unsigned k = kb; k < ke; ++k) {
            auto km = vmapk[k], kj = vj[km];
            auto k2 = lower_bound(vmapk.begin() + kb, vmapk.begin() + ke, kj) -
                      vmapk.begin();
            if (k2 > k) {
              if (k2 - k >= (ke - kb) / 2) {
                if (bestl > k2 - k) {
                  bestl = k2 - k;
                  ks = k;
                }
              }
            }
          }
        }
        auto k0 = vmapk[ks], k1 = vj[k0];
        if (k1 < k0) swap(k0, k1);
        graph::distance::DijkstraMask(g, ep, k0, max_cost, mask, vtd0f);
        graph::distance::DijkstraMask(ginv, ep, k0, max_cost, mask, vtd0i);
        graph::distance::DijkstraMask(g, ep, k1, max_cost, mask, vtd1f);
        graph::distance::DijkstraMask(ginv, ep, k1, max_cost, mask, vtd1i);
        for (unsigned q = qb; q < qe; ++q) {
          unsigned qm = vmapq[q];
          vbest[qm] = min(vbest[qm], min(vtd0i[vs[qm]] + vtd0f[ve[qm]],
                                         vtd1i[vs[qm]] + vtd1f[ve[qm]]));
        }
        auto isb0 = [&](unsigned i) { return (i < k0) || (!stl && (i > k1)); };
        auto isb1 = [&](unsigned i) { return (i > k0) && (i < k1); };
        auto isb2 = [&](unsigned i) { return stl && (i > k1); };
        unsigned newk0 = 0, newk1 = 0, newk2 = 0;
        for (unsigned k = kb; k < ke; ++k) {
          unsigned km = vmapk[k];
          if (isb0(km)) vtm0k[newk0++] = km;
          if (isb1(km)) vtm1k[newk1++] = km;
          if (isb2(km)) vtm2k[newk2++] = km;
        }
        unsigned newq0 = 0, newq1 = 0, newq2 = 0;
        for (unsigned q = qb; q < qe; ++q) {
          unsigned qm = vmapq[q];
          if (isb0(vs[qm]) && isb0(ve[qm])) vtm0q[newq0++] = qm;
          if (isb1(vs[qm]) && isb1(ve[qm])) vtm1q[newq1++] = qm;
          if (isb2(vs[qm]) && isb2(ve[qm])) vtm2q[newq2++] = qm;
        }
        for (unsigned k = 0; k < newk0; ++k) vmapk[kb + k] = vtm0k[k];
        for (unsigned k = 0; k < newk1; ++k) vmapk[kb + newk0 + k] = vtm1k[k];
        for (unsigned k = 0; k < newk2; ++k)
          vmapk[kb + newk0 + newk1 + k] = vtm2k[k];
        for (unsigned q = 0; q < newq0; ++q) vmapq[qb + q] = vtm0q[q];
        for (unsigned q = 0; q < newq1; ++q) vmapq[qb + newq0 + q] = vtm1q[q];
        for (unsigned q = 0; q < newq2; ++q)
          vmapq[qb + newq0 + newq1 + q] = vtm2q[q];
        Solve(kb, kb + newk0, qb, qb + newq0, !stl);
        Solve(kb + newk0, kb + newk0 + newk1, qb + newq0, qb + newq0 + newq1,
              !stl);
        Solve(kb + newk0 + newk1, kb + newk0 + newk1 + newk2,
              qb + newq0 + newq1, qb + newq0 + newq1 + newq2, !stl);
      }
    };

    Solve(0, K, 0, Q, true);
    cout << "Case #" << it << ": " << nvector::Sum(vbest) << endl;
  }
  return 0;
}
