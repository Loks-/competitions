#pragma once

namespace bst {
namespace subtree_data {

/**
 * @brief Gets subtree data for all nodes with keys less than the given end key.
 *
 * This function traverses the BST and accumulates subtree data for all nodes
 * whose keys are less than the given end key. The function now supports
 * order-dependent subtree data by using left/right-specific collection methods
 * to maintain the correct ordering of accumulated data.
 *
 * The function works in two phases:
 * Phase 1: Find the LCA (Lowest Common Ancestor) by traversing left until
 * finding a node with key < end.
 * Phase 2: Collect data using order-aware methods:
 * - Add the LCA node using set_node (establishes the base)
 * - Add left subtree data using add_left_subtree (maintains left ordering)
 * - Traverse right subtree: add right subtrees and nodes using
 * add_right_subtree and add_right_node to maintain right ordering
 *
 * This approach ensures that the accumulated data maintains the correct
 * inorder traversal ordering, which is crucial for order-dependent aggregators.
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
    Node* root, const typename Node::KeyType& end) {
  static_assert(ignore_assert || !Node::SubtreeDataType::use_tree_structure,
                "Data use tree structure.");
  typename Node::SubtreeDataType output;

  // Find the LCA
  for (; root; root = root->left) {
    root->apply_deferred();
    if (root->key < end) break;
  }
  if (!root) return output;
  output.set_node(root);

  // Collect data from the left subtree
  if (root->left) output.add_left_subtree(root->left);

  // Collect data from the right subtree
  for (root = root->right; root;) {
    root->apply_deferred();
    if (root->key < end) {
      if (root->left) output.add_right_subtree(root->left);
      output.add_right_node(root);
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
 * range [begin, end) while maintaining the correct ordering for order-dependent
 * subtree data. It uses a three-phase approach with order-aware collection:
 *
 * Phase 1: Find the common ancestor (LCA) of the range by traversing down
 * the tree until finding a node whose key is in the range [begin, end).
 *
 * Phase 2: Collect data from the left subtree using left-specific methods:
 * - Traverse left subtree: add right subtrees using add_left_subtree
 * - Add individual nodes using add_left_node to maintain left ordering
 * - This ensures left subtree data is collected in the correct order
 *
 * Phase 3: Collect data from the right subtree using right-specific methods:
 * - Traverse right subtree: add left subtrees using add_right_subtree
 * - Add individual nodes using add_right_node to maintain right ordering
 * - This ensures right subtree data is collected in the correct order
 *
 * The use of order-specific methods (add_left_subtree, add_right_subtree,
 * add_left_node, add_right_node) ensures that the accumulated data maintains
 * the proper inorder traversal ordering, which is essential for aggregators
 * that depend on the sequence of data collection.
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
    Node* root, const typename Node::KeyType& begin,
    const typename Node::KeyType& end) {
  static_assert(ignore_assert || !Node::SubtreeDataType::use_tree_structure,
                "Data use tree structure.");
  typename Node::SubtreeDataType output;
  if (!(begin < end)) return output;

  // Find the LCA
  for (; root;) {
    root->apply_deferred();
    if (root->key < begin) {
      root = root->right;
    } else if (!(root->key < end)) {
      root = root->left;
    } else {
      break;
    }
  }
  if (!root) return output;
  output.set_node(root);

  // Collect data from the left subtree
  for (Node* node = root->left; node;) {
    node->apply_deferred();
    if (node->key < begin) {
      node = node->right;
    } else {
      if (node->right) output.add_left_subtree(node->right);
      output.add_left_node(node);
      node = node->left;
    }
  }

  // Collect data from the right subtree
  for (Node* node = root->right; node;) {
    node->apply_deferred();
    if (node->key < end) {
      if (node->left) output.add_right_subtree(node->left);
      output.add_right_node(node);
      node = node->right;
    } else {
      node = node->left;
    }
  }

  return output;
}

}  // namespace subtree_data
}  // namespace bst
