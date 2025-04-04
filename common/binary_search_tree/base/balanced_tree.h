#pragma once

#include "common/binary_search_tree/base/extended_tree.h"
#include "common/binary_search_tree/base/remove_by_node.h"

namespace bst {
namespace base {
/**
 * @brief Base class for balanced binary search tree implementations.
 *
 * This class extends the base Tree class with balancing functionality.
 * It provides common implementations for balanced tree operations like
 * insertion, removal, and joining that maintain the tree's balance.
 *
 * Key features:
 * - Automatic rebalancing after insertions and removals
 * - Support for join operations with balance maintenance
 * - Efficient node removal from the rightmost position
 *
 * @tparam Derived The derived class type (for CRTP)
 */
template <class NodesManager, class Derived>
class BalancedTree : public ExtendedTree<NodesManager, Derived> {
 public:
  using Extended = ExtendedTree<NodesManager, Derived>;
  using Base = typename Extended::Base;
  using NodeType = typename Base::NodeType;
  using KeyType = typename Base::KeyType;

  friend Base;
  friend Extended;

  static constexpr bool support_join3 = Derived::support_join3;

  /**
   * @brief Constructs a balanced tree with the specified maximum number of
   * nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  explicit BalancedTree(size_t max_nodes) : Extended(max_nodes) {}

 protected:
  /**
   * @brief Fixes the balance of a node after an operation.
   *
   * This function should be implemented by derived classes to provide
   * specific balancing logic (e.g., AVL rotations, WAVL rebalancing).
   *
   * @param node The node that needs balance fixing
   * @return The new root of the subtree after balancing
   */
  static NodeType* fix_balance(NodeType* node);

  /**
   * @brief Fixes balance after insertion.
   *
   * By default, uses the same balancing logic as fix_balance.
   * Can be overridden by derived classes if insertion-specific
   * balancing is needed.
   */
  static NodeType* fix_balance_insert(NodeType* node) {
    return Derived::fix_balance(node);
  }

  /**
   * @brief Fixes balance after removal.
   *
   * By default, uses the same balancing logic as fix_balance.
   * Can be overridden by derived classes if removal-specific
   * balancing is needed.
   */
  static NodeType* fix_balance_remove(NodeType* node) {
    return Derived::fix_balance(node);
  }

  /**
   * @brief Implementation of insert operation with balance maintenance.
   */
  template <bool update_required>
  static NodeType* insert_impl(NodeType* root, NodeType* node) {
    if (!root) {
      if constexpr (update_required) node->update_subtree_data();
      return node;
    }
    root->apply_deferred();
    if (root->key < node->key) {
      root->set_right(insert_impl<update_required>(root->right, node));
    } else {
      root->set_left(insert_impl<update_required>(root->left, node));
    }
    root->update_subtree_data();
    return Derived::fix_balance_insert(root);
  }

  /**
   * @brief Implementation of node removal with balance maintenance.
   */
  static NodeType* remove_node_impl(NodeType* node) {
    NodeType *new_root, *fcn = nullptr;
    new_root = bst::base::RemoveByNode<NodeType, false>(node, fcn);
    if (!fcn) return new_root;
    for (NodeType* p = fcn->parent; p; p = (fcn = p)->parent) {
      NodeType* t = Derived::fix_balance_remove(fcn);
      if (t != fcn) {
        if (fcn == p->left) {
          p->set_left(t);
        } else {
          p->set_right(t);
        }
      }
    }
    return Derived::fix_balance_remove(fcn);
  }

  /**
   * @brief Removes the rightmost node from a subtree.
   *
   * This is a helper function used by join operations to extract
   * the rightmost node while maintaining balance.
   *
   * @param root The root of the subtree
   * @param removed_node Output parameter for the removed node
   * @return The new root of the subtree after removal
   */
  static NodeType* remove_right(NodeType* root, NodeType*& removed_node) {
    assert(root);
    root->apply_deferred();
    if (root->right) {
      root->set_right(remove_right(root->right, removed_node));
      root->update_subtree_data();
      return Derived::fix_balance_remove(root);
    } else {
      removed_node = root;
      NodeType* node = root->left;
      if (node) node->set_parent(nullptr);
      root->reset_links_and_update_subtree_data();
      return node;
    }
  }

  /**
   * @brief Implementation of join operation with balance maintenance.
   */
  static NodeType* join_impl(NodeType* l, NodeType* r) {
    static_assert(Derived::support_join3, "Join should be supported");
    if (!l) return r;
    if (!r) return l;
    NodeType* node = nullptr;
    l = Derived::remove_right(l, node);
    return Derived::join3(l, node, r);
  }
};
}  // namespace base
}  // namespace bst
