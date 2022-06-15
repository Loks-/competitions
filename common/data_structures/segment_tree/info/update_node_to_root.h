#pragma once

#include "common/binary_search_tree/info/update_node_to_root.h"

namespace ds {
namespace st {
namespace info {
template <class TNode>
inline void UpdateNodeToRoot(TNode* node) {
  bst::info::UpdateNodeToRoot(node);
}
}  // namespace info
}  // namespace st
}  // namespace ds
