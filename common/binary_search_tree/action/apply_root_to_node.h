#pragma once

#include "common/template.h"

#include <stack>

namespace bst {
namespace action {
namespace hidden {
template <class TNode>
constexpr void ApplyRootToNodeI(TNode*, TFakeFalse) {}

template <class TNode>
inline void ApplyRootToNodeI(TNode* node, TFakeTrue) {
  static_assert(TNode::use_parent, "use_parent should be true");
  thread_local std::stack<TNode*> s;
  for (; node; node = node->p) s.push(node);
  for (; !s.empty(); s.pop()) s.top()->ApplyAction();
}
}  // namespace hidden

template <class TNode>
inline void ApplyRootToNode(TNode* node) {
  hidden::ApplyRootToNodeI(node, TFakeBool<!TNode::TAction::is_none>{});
}

template <class TNode>
inline void ApplyRootToNode_ModifyData(TNode* node) {
  hidden::ApplyRootToNodeI(node, TFakeBool<TNode::TAction::modify_data>{});
}

template <class TNode>
inline void ApplyRootToNode_ModifyKeys(TNode* node) {
  hidden::ApplyRootToNodeI(node, TFakeBool<TNode::TAction::modify_keys>{});
}

template <class TNode>
inline void ApplyRootToNode_ModifyTree(TNode* node) {
  hidden::ApplyRootToNodeI(node, TFakeBool<TNode::TAction::modify_tree>{});
}
}  // namespace action
}  // namespace bst
