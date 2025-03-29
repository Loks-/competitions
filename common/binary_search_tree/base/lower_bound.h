#pragma once

namespace bst {
namespace base {

/**
 * @brief Finds the node with the lowest key greater than or equal to the given
 * key.
 *
 * This function traverses the tree to find a node with the lowest key that is
 * greater than or equal to the given key. If a node with the exact key exists,
 * it will be returned. Otherwise, it returns the node with the lowest key
 * that is greater than the given key.
 *
 * The function can optionally apply deferred computations to ensure the node's
 * state is up to date.
 *
 * @tparam apply_deferred Whether to apply deferred computations to the node.
 *                        Defaults to true to ensure node state is up to date
 *                        when accessed externally.
 * @tparam Node The BST node type (automatically deduced as const or non-const).
 * @param root The root of the subtree to search in.
 * @param key The key value to search for.
 * @return Pointer to the node with the lowest key greater than or equal to the
 *         given key, or nullptr if no such node exists.
 */
template <bool apply_deferred = true, typename Node>
[[nodiscard]] constexpr Node* lower_bound(Node* root,
                                          const typename Node::KeyType& key) {
  static_assert(Node::has_key, "has_key should be true");

  Node* last_greater = nullptr;
  for (Node* node = root; node;) {
    if constexpr (apply_deferred) {
      node->apply_deferred();
    }

    if (node->key < key) {
      node = node->right;
    } else if (key < node->key) {
      last_greater = node;
      node = node->left;
    } else {
      return node;
    }
  }
  return last_greater;
}

}  // namespace base
}  // namespace bst
