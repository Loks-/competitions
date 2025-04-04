#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/height.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/contiguous_nodes_manager.h"
#include "common/templates/tuple.h"

#include <tuple>

namespace bst {
template <bool use_parent, class TData,
          class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t>
class AVLTree
    : public base::BalancedTree<
          memory::ContiguousNodesManager<base::Node<
              TData,
              base::SubtreeData<templates::PrependIfMissingT<
                  subtree_data::Height, TAggregatorsTuple>>,
              base::Deferred<TDeferredTuple>, use_parent, true, TKey>>,
          AVLTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple, TKey>> {
 public:
  using TSubtreeData = base::SubtreeData<
      templates::PrependIfMissingT<subtree_data::Height, TAggregatorsTuple>>;
  using TDeferred = base::Deferred<TDeferredTuple>;
  using TNode =
      base::Node<TData, TSubtreeData, TDeferred, use_parent, true, TKey>;
  using TSelf =
      AVLTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple, TKey>;
  using TBTree =
      base::BalancedTree<memory::ContiguousNodesManager<TNode>, TSelf>;
  using Extended = typename TBTree::Extended;
  using Base = typename TBTree::Base;

  friend Base;
  friend Extended;
  friend TBTree;

  static constexpr bool support_join3 = true;

 public:
  explicit AVLTree(size_t max_nodes) : TBTree(max_nodes) {}

 protected:
  static constexpr int Height(const TNode* node) {
    return bst::subtree_data::height(node);
  }

  static constexpr int Balance(TNode* node) {
    return node ? Height(node->left) - Height(node->right) : 0;
  }

  static TNode* fix_balance(TNode* root) {
    if (Balance(root) == 2) {
      root->left->apply_deferred();
      if (Balance(root->left) == -1)
        base::rotate<false, true, TNode>(root->left->right, root->left, root);
      TNode* child = root->left;
      base::rotate<true, false, TNode>(child, root, nullptr);
      return child;
    } else if (Balance(root) == -2) {
      root->right->apply_deferred();
      if (Balance(root->right) == 1)
        base::rotate<false, true, TNode>(root->right->left, root->right, root);
      TNode* child = root->right;
      base::rotate<true, false, TNode>(child, root, nullptr);
      return child;
    }
    return root;
  }

  static TNode* Join3L(TNode* l, TNode* m1, TNode* r, int hr) {
    if (Height(l) > hr + 1) {
      l->apply_deferred();
      l->set_right(Join3L(l->right, m1, r, hr));
      l->update_subtree_data();
      return l;
    } else {
      return Extended::join3_impl(l, m1, r);
    }
  }

  static TNode* Join3R(TNode* l, TNode* m1, TNode* r, int hl) {
    if (Height(r) > hl + 1) {
      r->apply_deferred();
      r->set_left(Join3R(l, m1, r->left, hl));
      r->update_subtree_data();
      return r;
    } else {
      return Extended::join3_impl(l, m1, r);
    }
  }

 public:
  static TNode* join3(TNode* l, TNode* m1, TNode* r) {
    assert(m1 && !m1->left && !m1->right);
    const auto hl = Height(l), hr = Height(r), hd = hl - hr;
    return (hd > 1)    ? Join3L(l, m1, r, hr)
           : (hd < -1) ? Join3R(l, m1, r, hl)
                       : Extended::join3_impl(l, m1, r);
  }
};
}  // namespace bst
