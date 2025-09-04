#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace subtree_data {

/**
 * @brief Gets subtree data for the first end nodes in inorder traversal.
 *
 * This function traverses the BST and accumulates subtree data for the first
 * end nodes encountered during an inorder traversal. It efficiently collects
 * data by adding entire subtrees when possible, similar to how split_by_index
 * would work but without actually modifying the tree structure.
 *
 * The function now supports order-dependent subtree data by using left/right-
 * specific collection methods to maintain the correct ordering of accumulated
 * data. It works in three phases:
 *
 * Phase 1: Find the LCA (Lowest Common Ancestor) by traversing left until
 * finding a node that contains the index boundary.
 * Phase 2: Collect data using order-aware methods:
 * - Add the LCA node using set_node (establishes the base)
 * - Add left subtree data using add_left_subtree (maintains left ordering)
 * Phase 3: Traverse right subtree: add right subtrees and nodes using
 * add_right_subtree and add_right_node to maintain right ordering
 *
 * This approach ensures that the accumulated data maintains the correct
 * inorder traversal ordering, which is crucial for order-dependent aggregators.
 *
 * @tparam ignore_assert Whether to ignore the static assertion about tree
 * structure usage
 * @tparam Node The BST node type.
 * @param root The root of the BST to traverse.
 * @param end The number of nodes to collect from the beginning (exclusive).
 * @return The accumulated subtree data for the first end nodes.
 */
template <bool ignore_assert = false, typename Node>
[[nodiscard]] constexpr typename Node::SubtreeDataType get_by_index(
    Node* root, unsigned end) {
  static_assert(ignore_assert || !Node::SubtreeDataType::use_tree_structure,
                "Data use tree structure.");
  typename Node::SubtreeDataType output;
  if (!root || (end == 0)) return output;
  if (bst::subtree_data::size(root) <= end) return root->subtree_data;

  // Find the LCA
  for (; root; root = root->left) {
    root->apply_deferred();
    if (bst::subtree_data::size(root->left) < end) break;
  }
  assert(root);
  assert(end <= bst::subtree_data::size(root));
  if (bst::subtree_data::size(root) == end) return root->subtree_data;
  output.set_node(root);

  // Collect data from the left subtree
  if (root->left) output.add_left_subtree(root->left);
  end -= bst::subtree_data::size(root->left) + 1;

  // Collect data from the right subtree
  for (root = root->right; root;) {
    root->apply_deferred();
    const unsigned left_size = bst::subtree_data::size(root->left);
    if (left_size <= end) {
      if (root->left) output.add_right_subtree(root->left);
      end -= left_size;
      if (end == 0) break;
      output.add_right_node(root);
      end -= 1;
      if (end == 0) break;
      root = root->right;
    } else {
      root = root->left;
    }
  }

  assert(end == 0);
  return output;
}

/**
 * @brief Gets subtree data for nodes in an index-based range [begin, end).
 *
 * This function efficiently collects subtree data for nodes in the index range
 * [begin, end) while maintaining the correct ordering for order-dependent
 * subtree data. It uses a three-phase approach with order-aware collection:
 *
 * Phase 1: Find the common ancestor (LCA) of the range by traversing down
 * the tree based on subtree sizes until finding a node that contains the
 * range boundary.
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
 * @param begin The begin index (inclusive) for the range.
 * @param end The end index (exclusive) for the range.
 * @return The accumulated subtree data for nodes in index range [begin, end).
 */
template <bool ignore_assert = false, typename Node>
[[nodiscard]] constexpr typename Node::SubtreeDataType get_by_index(
    Node* root, unsigned begin, unsigned end) {
  static_assert(ignore_assert || !Node::SubtreeDataType::use_tree_structure,
                "Data use tree structure.");
  typename Node::SubtreeDataType output;
  if (!root || (begin >= end)) return output;
  if (bst::subtree_data::size(root) < end) end = bst::subtree_data::size(root);

  // Find the LCA
  for (; root;) {
    root->apply_deferred();
    const unsigned left_size = bst::subtree_data::size(root->left);
    if (left_size < begin) {
      root = root->right;
      begin -= left_size + 1;
      end -= left_size + 1;
    } else if (left_size >= end) {
      root = root->left;
    } else {
      break;
    }
  }
  assert(root);
  if ((begin == 0) && (bst::subtree_data::size(root) == end))
    return root->subtree_data;
  output.set_node(root);

  // Collect data from the left subtree
  for (Node* node = root->left; node;) {
    node->apply_deferred();
    const unsigned left_size = bst::subtree_data::size(node->left);
    if (left_size < begin) {
      node = node->right;
      begin -= left_size + 1;
    } else {
      if (node->right) output.add_left_subtree(node->right);
      output.add_left_node(node);
      node = node->left;
    }
  }

  // Collect data from the right subtree
  end -= bst::subtree_data::size(root->left) + 1;
  for (Node* node = root->right; node;) {
    node->apply_deferred();
    const unsigned left_size = bst::subtree_data::size(node->left);
    if (left_size <= end) {
      if (node->left) output.add_right_subtree(node->left);
      output.add_right_node(node);
      end -= left_size + 1;
      node = node->right;
    } else {
      node = node->left;
    }
  }

  assert(begin == 0);
  assert(end == 0);
  return output;
}

}  // namespace subtree_data
}  // namespace bst
