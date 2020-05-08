// https://www.hackerrank.com/challenges/dynamic-summation

#include "common/factorization/primes_list.h"
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

namespace {
class CashedPower {
 protected:
  vector<vector<vector<uint64_t>>> chains;

 public:
  CashedPower(uint64_t size) {
    chains.resize(size + 1);
    for (uint64_t m = 1; m <= size; ++m) {
      chains[m].resize(m);
      for (uint64_t a = 0; a < m; ++a) {
        auto& v = chains[m][a];
        v.push_back(0);
        v.push_back(1);
        for (uint64_t i = 0; i <= m; ++i) v.push_back((v.back() * a) % m);
        for (uint64_t i = v.size() - 2;; --i) {
          if (v[i] == v.back()) {
            v[0] = v.size() - 1 - i;
            break;
          }
        }
      }
    }
  }

  uint64_t Pow(uint64_t a, uint64_t b, uint64_t m) {
    auto& v = chains[m][a % m];
    ++b;
    if (b < v.size()) return v[b];
    uint64_t l = v[0];
    return v[v.size() + ((b - v.size()) % l) - l];
  }
};
}  // namespace

int main_dynamic_summation() {
  unsigned N, Q;
  cin >> N;
  TreeGraph tree(N);
  tree.ReadEdges();
  graph::TreeNodesInfo nodes_info(tree);
  TSTTree sttree(N);
  auto root = sttree.BuildTree(vector<TData>(N));
  TData d;
  CashedPower cp(M);
  factorization::PrimesList pl(M);
  vector<uint64_t> vpk;
  for (auto p : pl.GetPrimes()) {
    unsigned pk = p;
    for (; pk * p <= M;) pk *= p;
    vpk.push_back(pk);
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
    for (auto pk : vpk) {
      pr = TModular::MergeRemainders(p, pr, pk, cp.Pow(a, b, pk));
      p *= pk;
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
      d = FastPow(a, b) + FastPow(a + 1, b) + FastPow(b + 1, a);
      // d = LongUnsigned(a) * LongUnsigned(b);
      // d = LongUnsigned(0u);
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
