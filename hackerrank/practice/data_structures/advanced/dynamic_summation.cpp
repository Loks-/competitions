// https://www.hackerrank.com/challenges/dynamic-summation

#include "common/factorization/base.h"
#include "common/factorization/primes_list.h"
#include "common/factorization/utils/eulers_totient.h"
#include "common/graph/tree.h"
#include "common/graph/tree/nodes_info.h"
#include "common/numeric/long/modular.h"
#include "common/numeric/long/unsigned.h"
#include "common/segment_tree/action/add_each__sum.h"
#include "common/segment_tree/base/add_action_to_segment.h"
#include "common/segment_tree/base/get_segment_info.h"
#include "common/segment_tree/info/sum.h"
#include "common/segment_tree/segment_tree.h"
#include "common/stl/base.h"

static const unsigned M = 101;
using TData = LongUnsigned;
using TModular = numeric::nlong::Modular<false>;
using TSTTree = st::SegmentTree<TData, st::info::Sum<TData, st::info::None>,
                                st::action::AddEachSum<TData>>;

int main_dynamic_summation() {
  unsigned N, Q;
  cin >> N;
  TreeGraph tree(N);
  tree.ReadEdges();
  graph::TreeNodesInfo nodes_info(tree);
  TSTTree sttree(N);
  auto root = sttree.BuildTree(vector<TData>(N));
  TData d;

  factorization::PrimesList pl(M);
  vector<unsigned> vpk, vq, vqe;
  TFactorization modf;
  for (auto p : pl.GetPrimes()) {
    unsigned pk = p, k = 1;
    for (; pk * p <= M; ++k) pk *= p;
    modf.push_back({p, k});
    vpk.push_back(pk);
  }
  for (size_t i = 0, j = 0; i < modf.size(); i = j) {
    uint64_t q = 1;
    for (; (j < modf.size()); ++j) {
      if (q * vpk[j] > (1ull << 31)) break;
      q *= vpk[j];
    }
    vq.push_back(q);
    vqe.push_back(
        EulersTotient(q, TFactorization(modf.begin() + i, modf.begin() + j)));
  }

  auto ChildWithNode = [&](unsigned r, unsigned n) {
    if (nodes_info.parent[n] == r) return n;
    for (auto c : tree.Edges(r)) {
      if (c == nodes_info.parent[r]) continue;
      if (nodes_info.InsideSubtree(c, n)) return c;
    }
    assert(false);
    return n;
  };

  auto FastPow = [&](uint64_t a, uint64_t b) {
    LongUnsigned pr(0u), p(1u);
    for (size_t i = 0; i < vq.size(); ++i) {
      pr = TModular::MergeRemainders(
          p, pr, vq[i],
          (modular::TArithmetic_C32U::PowUSafe(
               a, (b > vqe[i]) ? vqe[i] + (b % vqe[i]) : b, vq[i]) +
           modular::TArithmetic_C32U::PowUSafe(
               a + 1, (b > vqe[i]) ? vqe[i] + (b % vqe[i]) : b, vq[i]) +
           modular::TArithmetic_C32U::PowUSafe(
               b + 1, (a > vqe[i]) ? vqe[i] + (a % vqe[i]) : a, vq[i])) %
              vq[i]);
      p *= vq[i];
    }
    return pr;
  };

  cin >> Q;
  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    char c;
    unsigned r, t, m;
    uint64_t a, b;
    cin >> c >> r >> t;
    r -= 1;
    t -= 1;
    if (c == 'R') {
      cin >> m;
      if (r == t) {
        d = root->info.sum;
      } else if (!nodes_info.InsideSubtree(t, r)) {
        d = st::GetSegmentInfo(
                root, nodes_info.preorder[t],
                nodes_info.preorder[t] + nodes_info.subtree_size[t] - 1)
                .sum;
      } else {
        auto c = ChildWithNode(t, r);
        d = st::GetSegmentInfo(root, 0, nodes_info.preorder[c] - 1).sum +
            st::GetSegmentInfo(
                root, nodes_info.preorder[c] + nodes_info.subtree_size[c],
                N - 1)
                .sum;
      }
      cout << d % m << endl;
    } else if (c == 'U') {
      cin >> a >> b;
      d = FastPow(a, b);
      if (r == t) {
        root->AddAction(d);
      } else if (!nodes_info.InsideSubtree(t, r)) {
        st::AddActionToSegment(
            root, nodes_info.preorder[t],
            nodes_info.preorder[t] + nodes_info.subtree_size[t] - 1, d);
      } else {
        auto c = ChildWithNode(t, r);
        st::AddActionToSegment(root, 0, nodes_info.preorder[c] - 1, d);
        st::AddActionToSegment(
            root, nodes_info.preorder[c] + nodes_info.subtree_size[c], N - 1,
            d);
      }
    }
  }
  return 0;
}
