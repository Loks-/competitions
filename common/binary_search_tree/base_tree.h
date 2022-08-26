#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/info/size.h"
#include "common/memory/nodes_manager_fixed_size.h"

namespace bst {
// Base BST without auto balance
template <bool use_parent, class TData, class TInfo = info::Size,
          class TAction = action::None, class TKey = int64_t>
class BaseTree
    : public base::BalancedTree<
          memory::NodesManagerFixedSize<
              base::Node<TData, TInfo, TAction, true, use_parent, TKey>>,
          BaseTree<use_parent, TData, TInfo, TAction, TKey>> {
 public:
  using TNode = base::Node<TData, TInfo, TAction, true, use_parent, TKey>;
  using TSelf = BaseTree<use_parent, TData, TInfo, TAction, TKey>;
  using TBTree =
      base::BalancedTree<memory::NodesManagerFixedSize<TNode>, TSelf>;
  friend TBTree;

  static const bool support_join3 = true;

 public:
  explicit BaseTree(size_t max_nodes) : TBTree(max_nodes) {}

 protected:
  static TNode* FixBalance(TNode* node) { return node; }

 public:
  static TNode* Join3(TNode* l, TNode* m1, TNode* r) {
    return TBTree::Join3IBase(l, m1, r);
  }
};
}  // namespace bst
