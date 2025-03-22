#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/subtree_data/height.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/nodes_manager_fixed_size.h"

namespace bst {
template <bool use_parent, class TData, class TInfo = subtree_data::Size,
          class TAction = action::None, class TKey = int64_t>
class AVLTree : public base::BalancedTree<
                    memory::NodesManagerFixedSize<
                        base::Node<TData, subtree_data::Height<TInfo>, TAction,
                                   true, use_parent, TKey>>,
                    AVLTree<use_parent, TData, TInfo, TAction, TKey>> {
 public:
  using TNode = base::Node<TData, subtree_data::Height<TInfo>, TAction, true,
                           use_parent, TKey>;
  using TSelf = AVLTree<use_parent, TData, TInfo, TAction, TKey>;
  using TBTree =
      base::BalancedTree<memory::NodesManagerFixedSize<TNode>, TSelf>;
  using TTree = typename TBTree::TTree;
  friend TBTree;
  friend TTree;

  static constexpr bool support_join3 = true;

 public:
  explicit AVLTree(size_t max_nodes) : TBTree(max_nodes) {}

 protected:
  static constexpr int Height(TNode* node) {
    return node ? int(node->subtree_data.height) : 0;
  }

  static constexpr int Balance(TNode* node) {
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

  static TNode* Join3L(TNode* l, TNode* m1, TNode* r, int hr) {
    if (Height(l) > hr + 1) {
      l->ApplyAction();
      l->SetR(Join3L(l->r, m1, r, hr));
      l->UpdateInfo();
      return l;
    } else {
      return TTree::Join3IBase(l, m1, r);
    }
  }

  static TNode* Join3R(TNode* l, TNode* m1, TNode* r, int hl) {
    if (Height(r) > hl + 1) {
      r->ApplyAction();
      r->SetL(Join3R(l, m1, r->l, hl));
      r->UpdateInfo();
      return r;
    } else {
      return TTree::Join3IBase(l, m1, r);
    }
  }

 public:
  static TNode* Join3(TNode* l, TNode* m1, TNode* r) {
    assert(m1 && !m1->l && !m1->r);
    const auto hl = Height(l), hr = Height(r), hd = hl - hr;
    return (hd > 1)    ? Join3L(l, m1, r, hr)
           : (hd < -1) ? Join3R(l, m1, r, hl)
                       : TTree::Join3IBase(l, m1, r);
  }
};
}  // namespace bst
