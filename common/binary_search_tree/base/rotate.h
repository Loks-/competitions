#pragma once

#include "common/base.h"

namespace bst {
namespace base {

/**
 * @brief Performs a rotation operation in a binary search tree.
 *
 * This function implements a single rotation that maintains the binary search
 * tree property. It can be used for both left and right rotations depending
 * on the relative positions of the nodes.
 *
 * The rotation operation:
 * 1. Updates parent-child relationships
 * 2. Maintains the binary search tree property
 * 3. Updates subtree data after the rotation
 * 4. Handles deferred computations
 *
 * @tparam update_child Whether to update subtree data on the child node
 * @tparam apply_deferred Whether to apply deferred computations on the child
 * node
 * @tparam TNode The type of the tree node
 * @param child The node to rotate (child of parent)
 * @param parent The parent node of the rotation
 * @param grandparent The grandparent node (parent of parent)
 */
template <bool update_child = true, bool apply_deferred = true, class TNode>
constexpr void rotate(TNode* child, TNode* parent, TNode* grandparent) {
  assert(child && parent);
  if constexpr (apply_deferred) child->apply_deferred();

  // Update grandparent's child pointer
  if (grandparent) {
    if (grandparent->left == parent) {
      grandparent->left = child;
    } else {
      grandparent->right = child;
    }
  }
  child->set_parent(grandparent);

  // Perform the rotation
  if (parent->left == child) {
    parent->set_left(child->right);
    child->set_right(parent);
  } else {
    parent->set_right(child->left);
    child->set_left(parent);
  }

  // Update subtree data
  parent->update_subtree_data();
  if constexpr (update_child) child->update_subtree_data();
}

/**
 * @brief Performs a rotation operation that moves a node up in the tree.
 *
 * This is a convenience function that performs a rotation operation
 * on a node and its parent. It's equivalent to calling rotate() with
 * the node, its parent, and its grandparent.
 *
 * @tparam update_child Whether to update subtree data on the child node
 * @tparam apply_deferred Whether to apply deferred computations on the child
 * node
 * @tparam TNode The type of the tree node
 * @param node The node to rotate up
 * @return true if the rotation was performed, false if the node has no parent
 */
template <bool update_child = true, bool apply_deferred = true, class TNode>
constexpr bool rotate_up(TNode* node) {
  static_assert(TNode::has_parent, "has_parent should be true");
  if (!node || !node->parent) return false;
  rotate<update_child, apply_deferred, TNode>(node, node->parent,
                                              node->parent->parent);
  return true;
}

}  // namespace base
}  // namespace bst
