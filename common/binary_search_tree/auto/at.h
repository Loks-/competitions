#pragma once

#include "common/binary_search_tree/base/at.h"

namespace bst {
namespace auto_ {

/**
 * @brief Gets the node at the specified inorder index.
 *
 * If TreeType has an at method, it will be used. Otherwise, falls back to
 * bst::base::at implementation.
 *
 * @tparam Tree The type of the tree
 * @tparam apply_deferred Whether to apply deferred operations before searching
 * @tparam TNode The type of the node
 * @param root The root of the tree
 * @param index The zero-based index to look up
 * @return Pointer to the node at the given index, or nullptr if index is out of
 * bounds
 */
template <typename Tree, bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr TNode* at(TNode*& root, size_t index) {
  if constexpr (requires { Tree::at; }) {
    return Tree::at(root, index);
  } else {
    return bst::base::at<apply_deferred>(root, index);
  }
}

}  // namespace auto_
}  // namespace bst
