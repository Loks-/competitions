#pragma once

#include "common/template.h"

namespace bst {
namespace info {
namespace hidden {
template <class TNode>
constexpr void UpdateNodeToRootI(TNode*, TMetaFalse) {}

template <class TNode>
inline void UpdateNodeToRootI(TNode* node, TMetaTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  for (; node; node = node->p) node->UpdateInfo();
}
}  // namespace hidden

template <class TNode>
inline void UpdateNodeToRoot(TNode* node) {
  hidden::UpdateNodeToRootI(node, TMetaBool<!TNode::TInfo::is_none>{});
}

template <class TNode>
inline void UpdateNodeToRoot_DataUpdated(TNode* node) {
  hidden::UpdateNodeToRootI(node, TMetaBool<TNode::TInfo::use_data>{});
}

template <class TNode>
inline void UpdateNodeToRoot_KeysUpdated(TNode* node) {
  hidden::UpdateNodeToRootI(node, TMetaBool<TNode::TInfo::use_keys>{});
}
}  // namespace info
}  // namespace bst
