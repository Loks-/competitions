#pragma once

namespace bst {
namespace base {

/**
 * @brief Finds the node with the highest key less than or equal to the given
 * key.
 *
 * This function traverses the tree to find a node with the highest key that is
 * less than or equal to the given key. If a node with the exact key exists,
 * it will be returned. Otherwise, it returns the node with the highest key
 * that is less than the given key.
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
 * @param key The key value to search for.
 * @return Pointer to the node with the highest key less than or equal to the
 *         given key, or nullptr if no such node exists.
 */
template <bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr TNode* floor(TNode* root,
                                     const typename TNode::KeyType& key) {
  static_assert(TNode::has_key, "has_key should be true");

  TNode* last_less = nullptr;
  for (TNode* node = root; node;) {
    if constexpr (apply_deferred) node->apply_deferred();

    if (node->key < key) {
      last_less = node;
      node = node->right;
    } else if (key < node->key) {
      node = node->left;
    } else {
      return node;
    }
  }
  return last_less;
}

}  // namespace base
}  // namespace bst
