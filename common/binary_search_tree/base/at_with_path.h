#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/size.h"

#include <vector>

namespace bst {
namespace base {

/**
 * @brief Gets a node at the specified position in the inorder sequence and
 * builds a path to it.
 *
 * This function is a variant of at that not only locates a node at the given
 * index but also builds a path from the root to that node. This is useful in
 * scenarios where:
 * - You need to know the exact path taken to reach the node at a specific index
 * - Parent links are not available and you need to reconstruct the path
 * - You plan to perform operations that require the path (e.g., path-based
 *   push-down operations)
 * - You want to analyze the traversal path for debugging or optimization
 *
 * The function traverses the tree using subtree size information to navigate
 * to the correct position in the inorder sequence. It records each node
 * visited along the path from root to the target node. The path includes the
 * root node and all intermediate nodes down to the target node (inclusive).
 *
 * @tparam apply_deferred Whether to apply deferred computations to nodes
 *                        during traversal. Defaults to true to ensure node
 *                        state is up to date when accessed.
 * @tparam TNode The BST node type (automatically deduced as const or
 * non-const).
 * @param root The root of the subtree to search in.
 * @param index The position in the inorder sequence (0-based).
 * @param path_to_node Vector that will be extended with the path from the
 *                     current root to the node at the specified index. The
 *                     path will include the root and all intermediate nodes
 *                     down to the target node (inclusive). If the index is
 *                     out of bounds, the path remains unchanged. Existing
 *                     path content is preserved.
 * @return Pointer to the node at the specified position, or nullptr if the
 *         index is out of bounds. The path is extended only if the index
 *         is valid.
 */
template <bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr TNode* at_with_path(TNode* root, size_t index,
                                            std::vector<TNode*>& path_to_node) {
  static_assert(TNode::SubtreeDataType::has_size,
                "subtree data should contain size");

  // Start building the path from the current root (don't clear existing path)

  if (!root) return nullptr;
  if (index >= bst::subtree_data::size(root)) return nullptr;

  for (TNode* node = root; node;) {
    // Add current node to the path
    path_to_node.push_back(node);

    if constexpr (apply_deferred) node->apply_deferred();

    const size_t ls = bst::subtree_data::size(node->left);
    if (index < ls) {
      node = node->left;
    } else if (index == ls) {
      // Found the node at the specified index
      return node;
    } else {
      index -= 1 + ls;
      node = node->right;
    }
  }

  // This should never be reached if the index is valid
  assert(false);
  return nullptr;
}

}  // namespace base
}  // namespace bst
