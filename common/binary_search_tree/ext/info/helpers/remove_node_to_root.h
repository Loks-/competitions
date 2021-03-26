#pragma once

#include "common/template.h"

namespace bst {
namespace ext {
namespace info {
namespace hidden {
template <class TNode>
inline void RemoveNodeToRootI(TNode*, TNode*, TFakeFalse) {}

template <class TNode>
inline void RemoveNodeToRootI(TNode* node, TNode* rem, TFakeTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  for (; node; node = node->p) node->RemoveInfo(rem);
}
}  // namespace hidden

template <class TNode>
inline void RemoveNodeToRoot(TNode* node, TNode* rem) {
  hidden::RemoveNodeToRootI(node, rem, TFakeBool<!TNode::TInfo::is_none>());
}
}  // namespace info
}  // namespace ext
}  // namespace bst
