#pragma once

namespace bst {
namespace base {

/**
 * @brief Finds a node with the exact key value in a subtree.
 *
 * This function traverses the tree to find a node with the exact key value.
 * It can optionally apply deferred computations to ensure the node's state
 * is up to date.
 *
 * The function automatically handles both const and non-const nodes through
 * template parameter deduction. When called with a const node and
 * apply_deferred=false, no deferred computations will be applied.
 *
 * @tparam apply_deferred Whether to apply deferred computations to the node.
 *                        Defaults to true to ensure node state is up to date
 *                        when accessed externally.
 * @tparam TNode The BST node type (automatically deduced as const or
 * non-const).
 * @param root The root of the subtree to search in.
 * @param key The key value to search for.
 * @return Pointer to the node with the exact key value, or nullptr if not
 * found.
 */
template <bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr TNode* find(TNode* root,
                                    const typename TNode::KeyType& key) {
  static_assert(TNode::has_key, "has_key should be true");

  for (TNode* node = root; node;) {
    if constexpr (apply_deferred) {
      node->apply_deferred();
    }

    if (node->key < key) {
      node = node->right;
    } else if (key < node->key) {
      node = node->left;
    } else {
      return node;
    }
  }
  return nullptr;
}

}  // namespace base
}  // namespace bst
