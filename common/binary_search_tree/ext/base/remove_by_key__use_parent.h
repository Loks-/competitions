#pragma once

#include "common/binary_search_tree/ext/base/remove_by_key__skip_update.h"
#include "common/binary_search_tree/subtree_data/utils/update_node_to_root.h"

namespace bst {
namespace ext {
namespace base {
template <class TNode, class TKey>
inline TNode* RemoveByKeyUseParent(TNode* root, const TKey& key,
                                   TNode*& removed_node,
                                   TNode*& first_changed_node) {
  TNode* new_root =
      RemoveByKeySkipUpdate(root, key, removed_node, first_changed_node);
  bst::subtree_data::update_node_to_root(first_changed_node);
  return new_root;
}
}  // namespace base
}  // namespace ext
}  // namespace bst
