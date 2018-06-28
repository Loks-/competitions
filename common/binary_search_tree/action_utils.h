#pragma once

#include "../template.h"

class BSTActionUtils
{
protected:
	template<class TNode>
	static void ApplyActionRootToNodeI(TNode* node, TFakeTrue) {}

	template<class TNode> 
	static void ApplyActionRootToNodeI(TNode * node, TFakeFalse)
	{
		stack<TNode*> s;
		for (; node; node = node->p) s.push(node);
		for (; !s.empty(); s.pop()) s.top()->ApplyAction();
	}

public:
	template<class TNode>
	static void ApplyActionRootToNode(TNode* node)
	{
		ApplyActionRootToNodeI(node, TFakeBool<TNode::TAction::is_none>());
	}
};
