#pragma once

#include "common/template.h"

#include <stack>

namespace ds {
namespace st {
namespace action {
namespace hidden {
template <class TNode>
constexpr void ApplyRootToNodeI(TNode*, MetaFalse) {}

template <class TNode>
inline void ApplyRootToNodeI(TNode* node, MetaTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  thread_local std::stack<TNode*> s;
  for (; node; node = node->p) s.push(node);
  for (; !s.empty(); s.pop()) s.top()->ApplyAction();
}
}  // namespace hidden

template <class TNode>
inline void ApplyRootToNode(TNode* node) {
  hidden::ApplyRootToNodeI(node, MetaBool<!TNode::TAction::is_none>{});
}
}  // namespace action
}  // namespace st
}  // namespace ds
