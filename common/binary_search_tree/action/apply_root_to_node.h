#pragma once

#include "common/template.h"

#include <stack>

namespace bst {
namespace action {
template <class TNode>
inline void ApplyRootToNode(TNode* node, TFakeFalse) {}

template <class TNode>
inline void ApplyRootToNode(TNode* node, TFakeTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  thread_local std::stack<TNode*> s;
  for (; node; node = node->p) s.push(node);
  for (; !s.empty(); s.pop()) s.top()->ApplyAction();
}

template <class TNode>
inline void ApplyRootToNode(TNode* node) {
  ApplyRootToNode(node, TFakeBool<!TNode::TAction::is_none>());
}
}  // namespace action
}  // namespace bst
