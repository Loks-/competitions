#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/size.h"

#include <utility>

namespace bst {
namespace base {

/**
 * @brief Merges two binary search trees into one.
 *
 * This function implements a merge operation for binary search trees that
 * maintains the binary search tree property. The merge is performed by:
 * 1. Selecting the root of the larger tree as the new root
 * 2. Splitting the smaller tree around the key of the new root
 * 3. Recursively merging the left and right subtrees
 *
 * The resulting tree maintains the following properties:
 * - All keys in the left subtree are less than the root's key
 * - All keys in the right subtree are greater than the root's key
 * - The inorder traversal of the tree contains all elements from both trees
 *
 * @tparam TreeType The type of the tree implementation (must provide split and
 * join3)
 * @tparam TNode The type of the tree node
 * @param root1 The root of the first tree
 * @param root2 The root of the second tree
 * @return Pointer to the root of the merged tree
 */
template <typename TreeType, typename TNode>
[[nodiscard]] static constexpr TNode* merge(TNode* root1, TNode* root2) {
  static_assert(TNode::has_key, "has_key should be true");
  // Handle empty tree cases
  if (!root1) return root2;
  if (!root2) return root1;

  // Verify that both nodes are actually roots of their trees
  assert(root1->is_root());
  assert(root2->is_root());

  // Optimize by choosing the larger tree as the root
  if constexpr (TNode::SubtreeDataType::has_size) {
    if (bst::subtree_data::size(root1) < bst::subtree_data::size(root2))
      std::swap(root1, root2);
  }

  // Apply any deferred operations on the root
  root1->apply_deferred();

  // Store and detach subtrees of the new root
  NodeType *r1l = root1->left, *r1r = root1->right, *r2l = nullptr,
           *r2r = nullptr;
  if (r1l) r1l->set_parent(nullptr);
  if (r1r) r1r->set_parent(nullptr);
  root1->set_left(nullptr);
  root1->set_right(nullptr);

  // Split the second tree around the root's key
  TreeType::split(root2, root1->key, r2l, r2r);

  // Recursively merge subtrees and join them with the root
  return TreeType::join3(merge<TreeType, TNode>(r1l, r2l), root1,
                         merge<TreeType, TNode>(r1r, r2r));
}

}  // namespace base
}  // namespace bst
