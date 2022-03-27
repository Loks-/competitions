#include "common/data_structures/unsigned_set.h"
#include "common/graph/graph.h"
#include "common/graph/graph/expand_components.h"
#include "common/graph/graph/strongly_connected_components.h"
#include "common/stl/base.h"
#include "common/vector/intersection.h"
#include "common/vector/read.h"

int main_swordmaster() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n, p;
    cin >> n >> p;
    vector<vector<unsigned>> va(n), vd(n);
    for (unsigned i = 0; i < n; ++i) {
      unsigned a, d;
      cin >> a >> d;
      va[i] = nvector::Read<unsigned>(a);
      vd[i] = nvector::Read<unsigned>(d);
    }
    bool possible = true;
    // Check A
    auto va2 = va;
    vector<unsigned> vd0(p + 1);
    for (auto d : vd[0]) vd0[d] = 1;
    ds::UnsignedSet usd(n);
    usd.InsertAll();
    usd.Remove(0);
    vector<unsigned> vat;
    for (bool bnext = true; bnext && !usd.Empty();) {
      bnext = false;
      auto vl = usd.List();
      for (auto u : vl) {
        for (auto a : va2[u]) {
          if (!vd0[a]) vat.push_back(a);
        }
        if (vat.empty()) {
          bnext = true;
          usd.Remove(u);
          for (auto d : vd[u]) vd0[d] = 1;
        } else {
          va2[u].swap(vat);
          vat.clear();
        }
      }
    }
    if (!usd.Empty()) possible = false;
    // Check D
    DirectedGraph g(n);
    for (unsigned u = 0; u < n; ++u) {
      for (unsigned v = 0; v < n; ++v) {
        if (v == u) continue;
        for (auto a : va[v]) {
          if (!binary_search(vd[u].begin(), vd[u].end(), a)) {
            g.AddEdge(u, v);
            break;
          }
        }
      }
    }
    auto scc = StronglyConnectedComponents(g);
    auto ecc = ExpandComponents(scc);
    for (unsigned c = 0; c < ecc.size(); ++c) {
      if (scc[0] == c) continue;
      bool leaf = true;
      for (auto u : ecc[c]) {
        for (auto v : g.Edges(u)) {
          if (scc[v] != c) {
            leaf = false;
            break;
          }
        }
        if (!leaf) break;
      }
      if (leaf) {
        auto vcd = vd[ecc[c][0]];
        for (unsigned i = 1; i < ecc[c].size(); ++i)
          vcd = nvector::IntersectionV(vcd, vd[ecc[c][i]]);
        bool block = true;
        for (auto u : ecc[c]) {
          bool f = false;
          for (auto a : va[u]) {
            if (binary_search(vcd.begin(), vcd.end(), a)) {
              f = true;
              break;
            }
          }
          if (!f) {
            block = false;
            break;
          }
        }
        if (block) {
          possible = false;
          break;
        }
      }
    }
    cout << "Case #" << it << ": " << (possible ? "YES" : "NO") << endl;
  }
  return 0;
}
