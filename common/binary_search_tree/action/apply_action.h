#pragma once

#include "../../base.h"
#include "../../template.h"

namespace {

template<class TNode>
void ApplyActionRootToNode(TNode* node, TFakeFalse) {}

template<class TNode>
void ApplyActionRootToNode(TNode * node, TFakeTrue)
{
	static_assert(TNode::use_parent, "use_parent should be true");
	stack<TNode*> s;
	for (; node; node = node->p) s.push(node);
	for (; !s.empty(); s.pop()) s.top()->ApplyAction();
}

template<class TNode>
void ApplyActionRootToNode(TNode* node) { ApplyActionRootToNode(node, TFakeBool<!TNode::TAction::is_none>()); }

} // namespace
