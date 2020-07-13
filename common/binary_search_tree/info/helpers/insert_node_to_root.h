#pragma once

#include "common/template.h"

namespace bst {
namespace info {
namespace hidden {
template <class TNode>
inline void InsertNodeToRootI(TNode*, TNode*, TFakeFalse) {}

template <class TNode>
inline void InsertNodeToRootI(TNode* node, TNode* ins, TFakeTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  for (; node; node = node->p) node->InsertInfo(ins);
}
}  // namespace hidden

template <class TNode>
inline void InsertNodeToRoot(TNode* node, TNode* ins) {
  hidden::InsertNodeToRootI(node, ins, TFakeBool<!TNode::TInfo::is_none>());
}
}  // namespace info
}  // namespace bst
