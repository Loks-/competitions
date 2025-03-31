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
 * @tparam TNode The type of the tree node
 * @param nodes Vector of node pointers in sorted order
 * @param first Starting index in the nodes vector (inclusive)
 * @param last Ending index in the nodes vector (exclusive)
 * @return Pointer to the root of the built tree
 */
template <typename TNode>
[[nodiscard]] constexpr TNode* build_tree(const std::vector<TNode*>& nodes,
                                          size_t first, size_t last) {
  if (first >= last) return nullptr;
  const size_t m = (first + last) / 2;
  TNode* root = nodes[m];
  root->set_left(build_tree(nodes, first, m));
  root->set_right(build_tree(nodes, m + 1, last));
  root->update_subtree_data();
  return root;
}

}  // namespace base
}  // namespace bst
