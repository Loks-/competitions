#pragma once

#include "common/template.h"

namespace bst {
namespace ext {
namespace info {
namespace hidden {
template <class TNode>
inline void RemoveNodeToRootI(TNode*, TNode*, TMetaFalse) {}

template <class TNode>
inline void RemoveNodeToRootI(TNode* node, TNode* rem, TMetaTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  for (; node; node = node->p) node->RemoveInfo(rem);
}
}  // namespace hidden

template <class TNode>
inline void RemoveNodeToRoot(TNode* node, TNode* rem) {
  hidden::RemoveNodeToRootI(node, rem, TMetaBool<!TNode::TInfo::is_none>());
}
}  // namespace info
}  // namespace ext
}  // namespace bst
