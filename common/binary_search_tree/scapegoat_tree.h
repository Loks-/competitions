#pragma once

#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/contiguous_nodes_manager.h"

namespace bst {
// In this implementation delete operation is different from wiki Scapegoat
// tree. It removes node from tree (similar to other trees), not just mark for
// future deletion.
template <bool use_parent, class TData,
          class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t>
class ScapegoatTree
    : public base::BalancedTree<
          memory::ContiguousNodesManager<base::Node<
              TData,
              base::SubtreeData<templates::PrependIfMissingT<
                  subtree_data::Size, TAggregatorsTuple>>,
              base::Deferred<TDeferredTuple>, use_parent, true, TKey>>,
          ScapegoatTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple,
                        TKey>> {
 public:
  // Height is less or equal to 2 * height(fully balanced tree).
  static constexpr double alpha = 0.7;

  using TSubtreeData = base::SubtreeData<
      templates::PrependIfMissingT<subtree_data::Size, TAggregatorsTuple>>;
  using TDeferred = base::Deferred<TDeferredTuple>;
  using TNode = base::Node<TData, TSubtreeData, base::Deferred<TDeferredTuple>,
                           use_parent, true, TKey>;
  using TSelf =
      ScapegoatTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple, TKey>;
  using TBTree =
      base::BalancedTree<memory::ContiguousNodesManager<TNode>, TSelf>;
  using Extended = typename TBTree::Extended;
  using Base = typename TBTree::Base;

  friend Base;
  friend Extended;
  friend TBTree;

  // Split/Join is supported but it's slow.
  static constexpr bool support_join3 = true;

 public:
  explicit ScapegoatTree(size_t max_nodes) : TBTree(max_nodes) {}

 protected:
  static void TraverseInorder(TNode* node, std::vector<TNode*>& output) {
    if (!node) return;
    node->apply_deferred();
    TraverseInorder(node->left, output);
    output.push_back(node);
    TraverseInorder(node->right, output);
  }

  static TNode* RebuildSubtree(TNode* node) {
    assert(node);
    std::vector<TNode*> nodes;
    TraverseInorder(node, nodes);
    return Base::template build_tree_impl<true>(nodes);
  }

  static TNode* fix_balance(TNode* node) {
    assert(node);
    const auto s = size_t(alpha * subtree_data::size(node));
    return ((subtree_data::size(node->left) > s) ||
            (subtree_data::size(node->right) > s))
               ? RebuildSubtree(node)
               : node;
  }

  static TNode* Join3L(TNode* l, TNode* m1, TNode* r, size_t rsize) {
    if (l) l->apply_deferred();
    if (subtree_data::size(l) > 2 * rsize) {
      l->set_right(Join3L(l->right, m1, r, rsize));
      l->update_subtree_data();
      return fix_balance(l);
    } else {
      return Extended::join3_base_impl(l, m1, r);
    }
  }

  static TNode* Join3R(TNode* l, TNode* m1, TNode* r, size_t lsize) {
    if (r) r->apply_deferred();
    if (subtree_data::size(r) > 2 * lsize) {
      r->set_left(Join3R(l, m1, r->left, lsize));
      r->update_subtree_data();
      return fix_balance(r);
    } else {
      return Extended::join3_base_impl(l, m1, r);
    }
  }

 public:
  static TNode* join3(TNode* l, TNode* m1, TNode* r) {
    assert(m1 && !m1->left && !m1->right);
    const size_t lsize = subtree_data::size(l), rsize = subtree_data::size(r);
    return lsize >= rsize ? Join3L(l, m1, r, rsize) : Join3R(l, m1, r, lsize);
  }
};
}  // namespace bst
