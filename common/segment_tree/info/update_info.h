#pragma once

#include "common/binary_search_tree/info/update_info.h"

template <class TNode>
inline void UpdateInfoNodeToRoot(TNode* node) {
  bst::UpdateInfoNodeToRoot(node);
}
