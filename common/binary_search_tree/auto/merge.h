#pragma once

#include "common/binary_search_tree/base/merge.h"

namespace bst {
namespace auto_ {

/**
 * @brief Merges two binary search trees into one.
 *
 * If TreeType has a merge method, it will be used. Otherwise, falls back to
 * bst::base::merge implementation.
 *
 * The merge operation combines two binary search trees while maintaining the
 * binary search tree property. The resulting tree contains all elements from
 * both input trees.
 *
 * @tparam Tree The type of the tree
 * @tparam TNode The type of the tree node
 * @param root1 The root of the first tree
 * @param root2 The root of the second tree
 * @return Pointer to the root of the merged tree
 */
template <typename Tree, typename TNode>
[[nodiscard]] constexpr TNode* merge(TNode* root1, TNode* root2) {
  if constexpr (requires { Tree::merge; }) return Tree::merge(root1, root2);
  return bst::base::merge<Tree, TNode>(root1, root2);
}

}  // namespace auto_
}  // namespace bst
