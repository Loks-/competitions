#pragma once

#include "../../template.h"
#include <stack>

template <class TNode>
inline void ApplyActionRootToNode(TNode* node, TFakeFalse) {}

template <class TNode>
inline void ApplyActionRootToNode(TNode* node, TFakeTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  std::stack<TNode*> s;
  for (; node; node = node->p) s.push(node);
  for (; !s.empty(); s.pop()) s.top()->ApplyAction();
}

template <class TNode>
inline void ApplyActionRootToNode(TNode* node) {
  ApplyActionRootToNode(node, TFakeBool<!TNode::TAction::is_none>());
}
