#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/swap.h"

namespace bst {
namespace base {

/**
 * @brief Pushes down a node by swapping it with its predecessor.
 *
 * This function is used in node removal operations to simplify the removal
 * process. It swaps the given node with its inorder predecessor (the rightmost
 * node in its left subtree) only if the node has both children.
 *
 * The push-down operation:
 * 1. Checks if the node has both left and right children
 * 2. If not, returns early (no push-down needed)
 * 3. Finds the rightmost node in the left subtree (predecessor)
 * 4. Swaps the current node with its predecessor
 * 5. Applies deferred computations during the search for the predecessor
 *
 * Note: This function does not update subtree data after the swap operation.
 * The caller is responsible for updating subtree data as needed after the
 * push-down operation is complete.
 *
 * This operation is commonly used in binary search tree removal algorithms
 * to convert a node with two children into a node with at most one child,
 * making the removal process simpler.
 *
 * @tparam TNode The BST node type
 * @param node The node to push down (must not be nullptr)
 */
template <class TNode>
constexpr void push_down(TNode* node) {
  assert(node);

  // Only perform push-down if the node has both children
  if (!node->left || !node->right) return;

  // Find the rightmost node in the left subtree (inorder predecessor)
  TNode* swap_node = node->left;
  for (swap_node->apply_deferred(); swap_node->right;
       swap_node->apply_deferred()) {
    swap_node = swap_node->right;
  }

  // Swap the current node with its predecessor
  swap_nodes(node, node->parent, swap_node, swap_node->parent);
}

}  // namespace base
}  // namespace bst
