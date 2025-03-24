#pragma once

#include "common/binary_search_tree/ext/base/insert_by_key__skip_update.h"
#include "common/binary_search_tree/subtree_data/utils/update_node_to_root.h"

namespace bst {
namespace ext {
namespace base {
template <class TNode>
inline void InsertByKeyUseParent(TNode* root, TNode* node) {
  InsertByKeySkipUpdate(root, node);
  bst::subtree_data::update_node_to_root(node);
}
}  // namespace base
}  // namespace ext
}  // namespace bst
