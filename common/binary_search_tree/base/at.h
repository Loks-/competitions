#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace base {

/**
 * @brief Gets a node at the specified position in the inorder sequence.
 *
 * This function traverses the tree to find a node at the given index in the
 * inorder sequence. The index is 0-based, where 0 represents the leftmost node
 * in the tree.
 *
 * The function can optionally apply deferred computations to ensure the node's
 * state is up to date.
 *
 * @tparam apply_deferred Whether to apply deferred computations to the node.
 *                        Defaults to true to ensure node state is up to date
 *                        when accessed externally.
 * @tparam TNode The BST node type (automatically deduced as const or
 * non-const).
 * @param root The root of the subtree to search in.
 * @param index The position in the inorder sequence (0-based).
 * @return Pointer to the node at the specified position, or nullptr if the
 *         index is out of bounds.
 */
template <bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr TNode* at(TNode* root, size_t index) {
  static_assert(TNode::SubtreeDataType::has_size,
                "subtree data should contain size");

  if (!root) return nullptr;
  if (index >= bst::subtree_data::size(root)) return nullptr;

  for (TNode* node = root; node;) {
    if constexpr (apply_deferred) node->apply_deferred();

    const size_t ls = bst::subtree_data::size(node->left);
    if (index < ls) {
      node = node->left;
    } else if (index == ls) {
      return node;
    } else {
      index -= 1 + ls;
      node = node->right;
    }
  }
  assert(false);
  return nullptr;
}

}  // namespace base
}  // namespace bst
