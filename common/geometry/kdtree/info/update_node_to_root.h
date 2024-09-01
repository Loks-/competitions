#pragma once

#include "common/template.h"

namespace geometry {
namespace kdtree {
namespace info {
namespace hidden {
template <class TNode>
constexpr void UpdateNodeToRootI(TNode*, TFakeFalse) {}

template <class TNode>
constexpr void UpdateNodeToRootI(TNode* node, TFakeTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  for (; node; node = node->p) node->UpdateInfo();
}
}  // namespace hidden

template <class TNode>
constexpr void UpdateNodeToRoot(TNode* node) {
  hidden::UpdateNodeToRootI(node, TFakeBool<!TNode::TInfo::is_none>{});
}

template <class TNode>
constexpr void UpdateNodeToRoot_DataUpdated(TNode* node) {
  hidden::UpdateNodeToRootI(node, TFakeBool<TNode::TInfo::use_data>{});
}
}  // namespace info
}  // namespace kdtree
}  // namespace geometry
