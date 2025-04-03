#pragma once

#include <vector>

namespace bst {
namespace base {

/**
 * @brief Builds a balanced binary search tree from a sorted vector of nodes.
 *
 * This function implements a recursive divide-and-conquer algorithm to build
 * a balanced binary search tree. It works by:
 * 1. Selecting the middle element as the root
 * 2. Recursively building the left subtree from elements before the middle
 * 3. Recursively building the right subtree from elements after the middle
 *
 * The resulting tree is balanced in terms of height, with the following
 * properties:
 * - The height difference between left and right subtrees is at most 1
 * - The inorder traversal of the tree matches the original vector order
 * - The tree maintains the binary search tree property
 *
 * @tparam update_leafs Whether to update subtree data for leaf nodes (default:
 * false)
 * @tparam TNode The type of the tree node
 * @param nodes Vector of node pointers in sorted order
 * @param first Starting index in the nodes vector (inclusive)
 * @param last Ending index in the nodes vector (exclusive)
 * @return Pointer to the root of the built tree
 */
template <bool update_leafs = false, typename TNode>
[[nodiscard]] constexpr TNode* build_tree(const std::vector<TNode*>& nodes,
                                          size_t first, size_t last) {
  if (first >= last) return nullptr;
  const size_t m = (first + last) / 2;
  TNode* root = nodes[m];
  root->set_left(build_tree<update_leafs, TNode>(nodes, first, m));
  root->set_right(build_tree<update_leafs, TNode>(nodes, m + 1, last));
  if (update_leafs || (last - first > 1)) root->update_subtree_data();
  return root;
}

/**
 * @brief Builds a balanced binary search tree from a sorted vector of nodes.
 *
 * This is a convenience wrapper around the main build_tree function that
 * handles the entire vector and sets the parent pointer of the root to nullptr.
 *
 * @tparam update_leafs Whether to update subtree data for leaf nodes (default:
 * false)
 * @tparam TNode The type of the tree node
 * @param nodes Vector of node pointers in sorted order
 * @return Pointer to the root of the built tree
 */
template <bool update_leafs = false, typename TNode>
[[nodiscard]] constexpr TNode* build_tree(const std::vector<TNode*>& nodes) {
  TNode* root = build_tree<update_leafs, TNode>(nodes, 0, nodes.size());
  if (root) root->set_parent(nullptr);
  return root;
}

}  // namespace base
}  // namespace bst
