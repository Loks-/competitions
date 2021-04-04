#include "common/binary_search_tree/base/traversal.h"
#include "common/binary_search_tree/treap.h"
#include "common/stl/base.h"
#include "common/vector/write.h"

using TTree = bst::Treap<false, true, unsigned, bst::info::Size>;
using TNode = TTree::TNode;

int main_median_sort() {
  unsigned T, N, Q, jr;
  cin >> T >> N >> Q;
  assert(N > 1);
  for (unsigned it = 0; it < T; ++it) {
    TTree tree(N);
    auto root = tree.Join(tree.New(1u), tree.New(2u));
    for (unsigned i = 3; i <= N; ++i) {
      unsigned l = 0, r = i - 1;
      for (; r > l;) {
        unsigned p1 = (2 * l + r) / 3, p2 = (l + 2 * r) / 3;
        if (p1 == p2) {
          if (p1 > 0)
            --p1;
          else
            ++p2;
        }
        unsigned i1 = TTree::FindByOrder(root, p1)->data,
                 i2 = TTree::FindByOrder(root, p2)->data;
        cout << i << " " << i1 << " " << i2 << endl;
        cin >> jr;
        if (jr == i1) {
          r = p1;
        } else if (jr == i) {
          l = p1 + 1;
          r = p2;
        } else if (jr == i2) {
          l = p2 + 1;
        } else {
          return -1;
        }
      }
      root = tree.InsertNewNodeByOrder(root, i, l);
    }
    auto v = bst::base::Traverse<TNode, TNode::TData>(
        root, bst::base::ETraversalOrder::Inorder);
    nvector::Write(v);
    cin >> jr;
    if (jr != 1) return -2;
  }
  return 0;
}
