#pragma once

#include "../template.h"

class BSTInfoUtils
{
protected:
	template<class TNode>
	static void UpdateInfoNodeToRootI(TNode* node, TFakeTrue) {}

	template<class TNode>
	static void UpdateInfoNodeToRootI(TNode * node, TFakeFalse)
	{
		for (; node; node = node->p) UpdateInfo(node);
	}

public:
	template<class TNode>
	static void UpdateInfoNodeToRoot(TNode* node)
	{
		UpdateInfoNodeToRootI(node, TFakeBool<TNode::TInfo::is_none>());
	}
};
