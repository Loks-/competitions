#pragma once

#include "common/base.h"
#include "common/binary_search_tree/ext/base/remove_right__skip_update.h"
#include "common/binary_search_tree/ext/info/helpers/remove_or_update_node_to_root.h"

namespace bst {
namespace ext {
namespace base {
template <class TNode>
inline void RemoveRightUseParent(TNode* root, TNode*& removed_node,
                                 TNode*& old_parent) {
  assert(root && !root->p);
  RemoveRightSkipUpdate(root, removed_node, old_parent);
  info::RemoveOrUpdateNodeToRoot(old_parent);
}
}  // namespace base
}  // namespace ext
}  // namespace bst
