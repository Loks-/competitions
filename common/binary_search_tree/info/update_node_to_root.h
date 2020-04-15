#pragma once

#include "common/template.h"

namespace bst {
namespace info {
template <class TNode>
inline void UpdateNodeToRoot(TNode* node, TFakeFalse) {}

template <class TNode>
inline void UpdateNodeToRoot(TNode* node, TFakeTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  for (; node; node = node->p) node->UpdateInfo();
}

template <class TNode>
inline void UpdateNodeToRoot(TNode* node) {
  UpdateNodeToRoot(node, TFakeBool<!TNode::TInfo::is_none>());
}
}  // namespace info
}  // namespace bst
