#pragma once

#include "common/binary_search_tree/base/find.h"
#include "common/binary_search_tree/base/remove_by_node.h"

namespace bst {
namespace base {
template <class TNode, class TKey>
inline TNode* RemoveByKey(TNode* root, const TKey& key, TNode*& removed_node,
                          TNode*& first_changed_node) {
  static_assert(TNode::use_key, "use_key should be true");
  static_assert(TNode::use_parent, "use_parent should be true");
  removed_node = find(root, key);
  if (!removed_node) return root;
  return RemoveByNode<TNode, false>(removed_node, first_changed_node);
}
}  // namespace base
}  // namespace bst
