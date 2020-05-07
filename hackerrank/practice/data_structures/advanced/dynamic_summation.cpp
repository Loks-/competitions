// https://www.hackerrank.com/challenges/dynamic-summation

#include "common/binary_search_tree/action/add_each__sum.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/sum.h"
#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/utils/add_action_to_segment.h"
#include "common/binary_search_tree/utils/segment_info_by_index.h"
#include "common/graph/tree.h"
#include "common/graph/tree/nodes_info.h"
#include "common/linear_algebra/vector_static_size.h"
#include "common/modular/arithmetic.h"
#include "common/stl/base.h"

static const unsigned M = 101;
using TData = la::VectorStaticSize<uint64_t, M>;
using TBSTree =
    bst::Treap<false, false, TData, bst::info::Sum<TData, bst::info::Size>,
               bst::action::AddEachSum<TData>>;
using TInfo = TBSTree::TInfo;

int main_dynamic_summation() {
  unsigned N, Q;
  cin >> N;
  TreeGraph tree(N);
  tree.ReadEdges();
  graph::TreeNodesInfo nodes_info(tree);
  TBSTree bstree(N);
  auto root = bstree.Build(vector<TData>(N));
  TInfo info;
  TData d;

  auto ChildWithNode = [&](unsigned r, unsigned n) {
    if (nodes_info.parent[n] == r) return n;
    for (auto c : tree.Edges(r)) {
      if (c == nodes_info.parent[r]) continue;
      if (nodes_info.InsideSubtree(c, n)) return c;
    }
    assert(false);
    return n;
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
        // bst::action::ApplyRootToNode(TBSTree::FindByOrder(root, t));
        root = bst::SegmentInfoByIndex<TBSTree>(
            root, nodes_info.preorder[t],
            nodes_info.preorder[t] + nodes_info.subtree_size[t], info);
        d = info.sum;
      } else {
        auto c = ChildWithNode(t, r);
        // bst::action::ApplyRootToNode(TBSTree::FindByOrder(c));
        root = bst::SegmentInfoByIndex<TBSTree>(root, 0, nodes_info.preorder[c],
                                                info);
        d = info.sum;
        if (nodes_info.preorder[c] + nodes_info.subtree_size[c] < N) {
          root = bst::SegmentInfoByIndex<TBSTree>(
              root, nodes_info.preorder[c] + nodes_info.subtree_size[c], N,
              info);
          d += info.sum;
        }
      }
      cout << d(m - 1) % m << endl;
    } else if (c == 'U') {
      cin >> a >> b;
      for (unsigned i = 0; i < M; ++i) {
        d(i) = (modular::TArithmetic_C32U::PowUSafe(a, b, i + 1) +
                modular::TArithmetic_C32U::PowUSafe(a + 1, b, i + 1) +
                modular::TArithmetic_C32U::PowUSafe(b + 1, a, i + 1)) %
               (i + 1);
        // d(i) = a * b % (i + 1);
      }
      if (r == t) {
        root->AddAction(d);
      } else if (!nodes_info.InsideSubtree(t, r)) {
        root = bst::AddActionToSegment<TBSTree>(
            root, nodes_info.preorder[t],
            nodes_info.preorder[t] + nodes_info.subtree_size[t], d);
      } else {
        auto c = ChildWithNode(t, r);
        root = bst::AddActionToSegment<TBSTree>(root, 0, nodes_info.preorder[c],
                                                d);
        root = bst::AddActionToSegment<TBSTree>(
            root, nodes_info.preorder[c] + nodes_info.subtree_size[c], N, d);
        // root->AddAction(d);
        // for (unsigned i = 0; i < M; ++i) d(i) = i + 1 - d(i);
        // root = bst::AddActionToSegment<TBSTree>(
        //     root, nodes_info.preorder[t] + 1,
        //     nodes_info.preorder[t] + nodes_info.subtree_size[t], d);
      }
    }
  }
  return 0;
}
