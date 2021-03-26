#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/info/height.h"
#include "common/binary_search_tree/info/size.h"
#include "common/nodes_manager_fixed_size.h"

namespace bst {
template <bool use_parent, class TData, class TInfo = info::Size,
          class TAction = action::None, class TKey = int64_t>
class AVLTree
    : public base::BalancedTree<
          NodesManagerFixedSize<base::Node<TData, info::Height<TInfo>, TAction,
                                           true, use_parent, TKey>>,
          AVLTree<use_parent, TData, TInfo, TAction, TKey>> {
 public:
  using TNode =
      base::Node<TData, info::Height<TInfo>, TAction, true, use_parent, TKey>;
  using TSelf = AVLTree<use_parent, TData, TInfo, TAction, TKey>;
  using TBTree = base::BalancedTree<NodesManagerFixedSize<TNode>, TSelf>;
  using TTree = typename TBTree::TTree;
  friend TBTree;
  friend TTree;

 public:
  explicit AVLTree(size_t max_nodes) : TBTree(max_nodes) {}

 protected:
  static int Height(TNode* node) { return node ? int(node->info.height) : 0; }

  static int Balance(TNode* node) {
    return node ? Height(node->l) - Height(node->r) : 0;
  }

  static TNode* FixBalance(TNode* root) {
    if (Balance(root) == 2) {
      root->l->ApplyAction();
      if (Balance(root->l) == -1)
        base::Rotate<TNode, false, true>(root->l->r, root->l, root);
      TNode* child = root->l;
      base::Rotate<TNode, true, false>(child, root, nullptr);
      return child;
    } else if (Balance(root) == -2) {
      root->r->ApplyAction();
      if (Balance(root->r) == 1)
        base::Rotate<TNode, false, true>(root->r->l, root->r, root);
      TNode* child = root->r;
      base::Rotate<TNode, true, false>(child, root, nullptr);
      return child;
    }
    return root;
  }
};
}  // namespace bst
