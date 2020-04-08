#pragma once

#include "common/base.h"
#include "common/template.h"

#include <vector>

namespace bst {
template <class TNode>
inline void UpdateInfoNodeToRootWithPath(
    const std::vector<TNode*>& node_to_root_path, size_t start_from_index,
    TFakeFalse) {}

template <class TNode>
inline void UpdateInfoNodeToRootWithPath(
    const std::vector<TNode*>& node_to_root_path, size_t start_from_index,
    TFakeTrue) {
  for (auto it = node_to_root_path.begin() + start_from_index;
       it != node_to_root_path.end(); ++it)
    (*it)->UpdateInfo();
}

template <class TNode>
inline void UpdateInfoNodeToRootWithPath(
    const std::vector<TNode*>& node_to_root_path, size_t start_from_index = 0) {
  UpdateInfoNodeToRootWithPath(node_to_root_path, start_from_index,
                               TFakeBool<!TNode::TInfo::is_none>());
}
}  // namespace bst
