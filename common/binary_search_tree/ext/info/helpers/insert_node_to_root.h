#pragma once

#include "common/template.h"

namespace bst {
namespace ext {
namespace info {
namespace hidden {
template <class TNode>
inline void InsertNodeToRootI(TNode*, TNode*, MetaFalse) {}

template <class TNode>
inline void InsertNodeToRootI(TNode* node, TNode* ins, MetaTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  for (; node; node = node->p) node->InsertInfo(ins);
}
}  // namespace hidden

template <class TNode>
inline void InsertNodeToRoot(TNode* node, TNode* ins) {
  hidden::InsertNodeToRootI(node, ins, MetaBool<!TNode::TInfo::is_none>());
}
}  // namespace info
}  // namespace ext
}  // namespace bst
