#pragma once

#include "common/binary_search_tree/base/find.h"

namespace bst {
namespace auto_ {

/**
 * @brief Finds a node with the given key in the tree.
 *
 * If TreeType has a find method, it will be used. Otherwise, falls back to
 * bst::base::find implementation.
 *
 * @tparam Tree The type of the tree
 * @tparam apply_deferred Whether to apply deferred operations before searching
 * @tparam TNode The type of the node
 * @param root The root of the tree to search in
 * @param key The key to search for
 * @return Pointer to the node with the given key, or nullptr if not found
 */
template <typename Tree, bool apply_deferred = true, typename TNode>
[[nodiscard]] constexpr TNode* find(TNode*& root,
                                    const typename TNode::KeyType& key) {
  if constexpr (requires { Tree::find; }) {
    return Tree::find(root, key);
  } else {
    return bst::base::find<apply_deferred>(root, key);
  }
}

}  // namespace auto_
}  // namespace bst
