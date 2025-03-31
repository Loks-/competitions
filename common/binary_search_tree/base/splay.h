#pragma once

#include "common/binary_search_tree/base/rotate.h"

namespace bst {
namespace base {

/**
 * @brief Performs a splay operation on a node in a binary search tree.
 *
 * The splay operation moves a node to the root of the tree through a series
 * of rotations while maintaining the binary search tree property. This
 * operation is fundamental to splay trees and helps maintain balance through
 * amortized analysis.
 *
 * The splay operation consists of three cases:
 * 1. Zig: When the parent is the root (single rotation)
 * 2. Zig-Zig: When the node and its parent are both left or both right children
 * 3. Zig-Zag: When the node and its parent are on opposite sides
 *
 * The operation is optimized to:
 * - Minimize data updates during rotations
 * - Apply deferred computations only at the end
 * - Handle all cases efficiently
 *
 * @tparam TNode The type of the tree node
 * @param node The node to splay to the root
 */
template <typename TNode>
constexpr void splay(TNode* node) {
  if (!node) return;

  while (TNode* parent = node->parent) {
    TNode* grandparent = parent->parent;

    // Zig case: parent is root
    if (!grandparent) {
      rotate<false, false, TNode>(node, parent, grandparent);
      break;
    }

    // Determine if we have a Zig-Zig or Zig-Zag case
    const bool is_zig_zig =
        ((grandparent->left == parent) == (parent->left == node));

    // For Zig-Zig, rotate parent first, then node
    // For Zig-Zag, rotate node twice
    rotate_up<false, false>(is_zig_zig ? parent : node);
    rotate_up<false, false>(node);
  }

  // Update subtree data at the end
  node->update_subtree_data();
}

}  // namespace base
}  // namespace bst
