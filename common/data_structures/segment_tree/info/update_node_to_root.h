#pragma once

#include "common/binary_search_tree/subtree_data/update_node_to_root.h"

namespace ds {
namespace st {
namespace info {
template <class TNode>
inline void UpdateNodeToRoot(TNode* node) {
  bst::subtree_data::update_node_to_root(node);
}
}  // namespace info
}  // namespace st
}  // namespace ds
