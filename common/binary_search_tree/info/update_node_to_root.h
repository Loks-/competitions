#pragma once

#include "common/template.h"

namespace bst {
namespace info {
namespace hidden {
template <class TNode>
inline void UpdateNodeToRootI(TNode*, TFakeFalse) {}

template <class TNode>
inline void UpdateNodeToRootI(TNode* node, TFakeTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  for (; node; node = node->p) node->UpdateInfo();
}
}  // namespace hidden

template <class TNode>
inline void UpdateNodeToRoot(TNode* node) {
  hidden::UpdateNodeToRootI(node, TFakeBool<!TNode::TInfo::is_none>());
}
}  // namespace info
}  // namespace bst
