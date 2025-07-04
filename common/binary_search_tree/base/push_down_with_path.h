#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/swap.h"

#include <vector>

namespace bst {
namespace base {

/**
 * @brief Pushes down a node by swapping it with its predecessor using a path.
 *
 * This function is a variant of push_down that operates when parent links are
 * not available. Instead of using parent pointers, it uses a pre-computed path
 * from the root to the target node. This is useful in scenarios where:
 * - Parent links are not maintained in the tree structure
 * - The path to the node has already been computed for other purposes
 * - Memory efficiency is important (avoiding parent pointer overhead)
 *
 * The push-down operation:
 * 1. Checks if the node has both left and right children
 * 2. If not, returns early (no push-down needed)
 * 3. Finds the rightmost node in the left subtree (predecessor)
 * 4. Updates the path to include the path to the predecessor
 * 5. Swaps the current node with its predecessor using path-based parent lookup
 * 6. Updates the path to reflect the new positions after the swap
 *
 * Note: This function does not update subtree data after the swap operation.
 * The caller is responsible for updating subtree data as needed after the
 * push-down operation is complete.
 *
 * The path is modified in-place to reflect the new tree structure after the
 * swap operation. The path will contain the updated path to the node that
 * was originally at the target position.
 *
 * @tparam TNode The BST node type
 * @param path_to_node Vector containing the path from root to the target node
 *                     (inclusive). The path is modified to reflect the new
 *                     tree structure after the swap operation.
 */
template <class TNode>
constexpr void push_down_with_path(std::vector<TNode*>& path_to_node) {
  assert(!path_to_node.empty());
  const unsigned index = path_to_node.size() - 1;
  TNode* node = path_to_node.back();

  // Only perform push-down if the node has both children
  if (!node->left || !node->right) return;

  // Find the rightmost node in the left subtree (inorder predecessor)
  // and build the path to it
  TNode* swap_node = node->left;
  for (swap_node->apply_deferred(); swap_node->right;
       swap_node->apply_deferred()) {
    path_to_node.push_back(swap_node);
    swap_node = swap_node->right;
  }

  // Swap the current node with its predecessor using path-based parent lookup.
  // The parent of the original node is at path_to_node[index - 1] (if index >
  // 0). The parent of the swap node is the last node in the path
  // (path_to_node.back()).
  swap_nodes(node, (index > 0) ? path_to_node[index - 1] : nullptr, swap_node,
             path_to_node.back());

  // Update path to reflect the new positions after the swap:
  // - The original position now contains the swap node
  // - The swap node's original position now contains the original node
  path_to_node[index] = swap_node;
  path_to_node.push_back(node);
}

}  // namespace base
}  // namespace bst
