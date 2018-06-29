#pragma once

#include "../template.h"

class BSTActionNone
{
public:
	using TSelf = BSTActionNone;
	static const bool is_none = true;

	bool IsEmpty() const { return true; }
	void Set(const TSelf& new_action) {}
	template<class TNode>
	void Apply(const TNode* node) {}
};

namespace {

template<class TNode>
void ApplyActionRootToNode(TNode* node, TFakeFalse) {}

template<class TNode>
void ApplyActionRootToNode(TNode * node, TFakeTrue)
{
	stack<TNode*> s;
	for (; node; node = node->p) s.push(node);
	for (; !s.empty(); s.pop()) s.top()->ApplyAction();
}

template<class TNode>
void ApplyActionRootToNode(TNode* node)
{
	ApplyActionRootToNode(node, TFakeBool<!TNode::TAction::is_none>());
}

} // namespace
