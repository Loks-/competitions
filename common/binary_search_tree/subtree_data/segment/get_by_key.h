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
inline typename Node::TInfo get_by_key(const Node* root,
                                       const typename Node::TKey& end) {
  typename Node::TInfo output;
  for (; root;) {
    if (root->key < end) {
      if (root->l) output.add_subtree(root->l);
      output.add_node(root);
      root = root->r;
    } else {
      root = root->l;
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
inline typename Node::TInfo get_by_key(const Node* root,
                                       const typename Node::TKey& begin,
                                       const typename Node::TKey& end) {
  for (; root;) {
    if (root->key < begin) {
      root = root->r;
    } else if (root->key >= end) {
      root = root->l;
    } else {
      break;
    }
  }
  typename Node::TInfo output;
  if (!root) return output;
  output.set_node(root);
  for (const Node* node = root->l; node;) {
    if (node->key < begin) {
      node = node->r;
    } else {
      output.add_node(node);
      if (node->r) output.add_subtree(node->r);
      node = node->l;
    }
  }
  for (const Node* node = root->r; node;) {
    if (node->key < end) {
      if (node->l) output.add_subtree(node->l);
      output.add_node(node);
      node = node->r;
    } else {
      node = node->l;
    }
  }
  return output;
}

}  // namespace segment
}  // namespace subtree_data
}  // namespace bst
