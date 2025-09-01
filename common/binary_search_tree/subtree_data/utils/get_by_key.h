#pragma once

namespace bst {
namespace subtree_data {

/**
 * @brief Gets subtree data for all nodes with keys less than the given end key.
 *
 * This function traverses the BST and accumulates subtree data for all nodes
 * whose keys are less than the given end key. The traversal follows a specific
 * pattern: when a node's key is less than the end key, it adds the node's left
 * subtree (if it exists) and the node itself, then moves to the right child.
 * When a node's key is >= the end key, it moves to the left child.
 *
 * The function accumulates data by:
 * - Adding left subtrees of nodes with keys < end (using add_subtree)
 * - Adding individual nodes with keys < end (using add_node)
 * - Skipping nodes with keys >= end
 *
 * @tparam ignore_assert Whether to ignore the static assertion about tree
 * structure usage
 * @tparam Node The BST node type.
 * @param root The root of the BST to traverse.
 * @param end The end key (exclusive) for the range.
 * @return The accumulated subtree data for nodes with keys less than end.
 */
template <bool ignore_assert = false, typename Node>
[[nodiscard]] constexpr typename Node::SubtreeDataType get_by_key(
    const Node* root, const typename Node::KeyType& end) {
  static_assert(ignore_assert || !Node::SubtreeDataType::use_tree_structure,
                "Data use tree structure.");
  typename Node::SubtreeDataType output;
  for (; root;) {
    if (root->key < end) {
      if (root->left) output.add_subtree(root->left);
      output.add_node(root);
      root = root->right;
    } else {
      root = root->left;
    }
  }
  return output;
}

/**
 * @brief Gets subtree data for all nodes with keys in the given range [begin,
 * end).
 *
 * This function efficiently collects subtree data for nodes with keys in the
 * range [begin, end) by using a two-phase approach:
 *
 * Phase 1: Find the common ancestor (LCA) of the range by traversing down
 * the tree until finding a node whose key is in the range [begin, end).
 *
 * Phase 2: Collect data from the LCA and its subtrees:
 * - Add the LCA node itself
 * - Traverse left subtree: add nodes >= begin and their right subtrees
 * - Traverse right subtree: add nodes < end and their left subtrees
 *
 * The traversal ensures that all nodes in the range are included while
 * efficiently adding entire subtrees when possible.
 *
 * @tparam ignore_assert Whether to ignore the static assertion about tree
 * structure usage
 * @tparam Node The BST node type.
 * @param root The root of the BST to traverse.
 * @param begin The begin key (inclusive) for the range.
 * @param end The end key (exclusive) for the range.
 * @return The accumulated subtree data for nodes with keys in [begin, end).
 */
template <bool ignore_assert = false, typename Node>
[[nodiscard]] constexpr typename Node::SubtreeDataType get_by_key(
    const Node* root, const typename Node::KeyType& begin,
    const typename Node::KeyType& end) {
  static_assert(ignore_assert || !Node::SubtreeDataType::use_tree_structure,
                "Data use tree structure.");
  for (; root;) {
    if (root->key < begin) {
      root = root->right;
    } else if (root->key >= end) {
      root = root->left;
    } else {
      break;
    }
  }
  typename Node::SubtreeDataType output;
  if (!root) return output;
  output.set_node(root);
  for (const Node* node = root->left; node;) {
    if (node->key < begin) {
      node = node->right;
    } else {
      output.add_node(node);
      if (node->right) output.add_subtree(node->right);
      node = node->left;
    }
  }
  for (const Node* node = root->right; node;) {
    if (node->key < end) {
      if (node->left) output.add_subtree(node->left);
      output.add_node(node);
      node = node->right;
    } else {
      node = node->left;
    }
  }
  return output;
}

}  // namespace subtree_data
}  // namespace bst
