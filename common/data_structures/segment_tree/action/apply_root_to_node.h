#pragma once

#include "common/binary_search_tree/action/apply_root_to_node.h"

namespace ds {
namespace st {
namespace action {
template <class TNode>
inline void ApplyRootToNode(TNode* node) {
  bst::action::ApplyRootToNode(node);
}
}  // namespace action
}  // namespace st
}  // namespace ds
