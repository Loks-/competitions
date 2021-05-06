#pragma once

#include "common/binary_search_tree/info/update_node_to_root.h"

namespace geometry {
namespace kdtree {
namespace info {
template <class TNode>
inline void UpdateNodeToRoot(TNode* node) {
  bst::info::UpdateNodeToRoot(node);
}
}  // namespace info
}  // namespace kdtree
}  // namespace geometry
