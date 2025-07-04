#pragma once

#include "common/base.h"

#include <vector>

namespace bst {
namespace base {

/**
 * @brief Finds a node with the exact key value and builds a path to it.
 *
 * This function is a variant of find that not only locates a node with the
 * specified key but also builds a path from the root to that node. This is
 * useful in scenarios where:
 * - You need to know the exact path taken to reach the node
 * - Parent links are not available and you need to reconstruct the path
 * - You plan to perform operations that require the path (e.g., path-based
 *   push-down operations)
 * - You want to analyze the search path for debugging or optimization
 *
 * The function traverses the tree following the binary search tree property
 * and records each node visited along the path from root to the target node.
 * The path includes the root node and all intermediate nodes down to the
 * target node (inclusive).
 *
 * @tparam apply_deferred Whether to apply deferred computations to nodes
 *                        during traversal. Defaults to true to ensure node
 *                        state is up to date when accessed.
 * @tparam TNode The BST node type (automatically deduced as const or
 * non-const).
 * @param root The root of the subtree to search in.
 * @param key The key value to search for.
 * @param path_to_node Vector that will be extended with the path from the
 *                     current root to the found node. The path will include
 *                     the root and all intermediate nodes down to the target
 *                     node (inclusive). If the key is not found, the path
 *                     will contain the search path to where the key would be
 *                     inserted. Existing path content is preserved.
 * @return Pointer to the node with the exact key value, or nullptr if not
 *         found. The path is always extended regardless of whether the node
 *         is found.
 */
template <bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr TNode* find_with_path(
    TNode* root, const typename TNode::KeyType& key,
    std::vector<TNode*>& path_to_node) {
  static_assert(TNode::has_key, "has_key should be true");

  // Start building the path from the current root (don't clear existing path)

  for (TNode* node = root; node;) {
    // Add current node to the path
    path_to_node.push_back(node);

    if constexpr (apply_deferred) node->apply_deferred();

    if (node->key < key) {
      node = node->right;
    } else if (key < node->key) {
      node = node->left;
    } else {
      // Found the node with the exact key
      return node;
    }
  }

  // Key not found, but path contains the search path to where it would be
  return nullptr;
}

}  // namespace base
}  // namespace bst
