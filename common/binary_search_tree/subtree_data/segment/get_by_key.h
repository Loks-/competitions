#pragma once

namespace bst {
namespace subtree_data {
namespace segment {

/**
 * @brief Gets subtree data for all nodes with keys less than the given end key.
 *
 * This function traverses the BST and accumulates subtree data for all nodes
 * whose keys are less than the given end key. It adds both individual nodes
 * and their left subtrees to the result.
 *
 * @tparam Node The BST node type.
 * @param root The root of the BST to traverse.
 * @param end The end key (exclusive) for the range.
 * @return The accumulated subtree data for nodes with keys less than end.
 */
template <typename Node>
inline typename Node::SubtreeDataType get_by_key(
    const Node* root, const typename Node::KeyType& end) {
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
 * @brief Gets subtree data for all nodes with keys in the given range.
 *
 * This function traverses the BST and accumulates subtree data for all nodes
 * whose keys are in the range [begin, end). It first finds the common
 * ancestor of nodes in the range, then traverses its left and right subtrees
 * to collect all relevant nodes and their subtrees.
 *
 * @tparam Node The BST node type.
 * @param root The root of the BST to traverse.
 * @param begin The begin key (inclusive) for the range.
 * @param end The end key (exclusive) for the range.
 * @return The accumulated subtree data for nodes with keys in [begin, end).
 */
template <typename Node>
inline typename Node::SubtreeDataType get_by_key(
    const Node* root, const typename Node::KeyType& begin,
    const typename Node::KeyType& end) {
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

}  // namespace segment
}  // namespace subtree_data
}  // namespace bst
