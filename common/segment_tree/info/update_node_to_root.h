#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/update_info.h"

namespace st {
namespace info {
template <class TNode>
inline void UpdateNodeToRoot(TNode* node) {
  bst::UpdateInfoNodeToRoot(node);
}
}  // namespace info
}  // namespace st
