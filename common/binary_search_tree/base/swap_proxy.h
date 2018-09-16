#pragma once

#include "swap.h"
#include "../action/apply_action.h"
#include "../info/update_info.h"
#include "../../base.h"

template<class TNode, bool update_info, bool apply_action>
inline void BSTSwapProxy(TNode* node1, TNode* node2)
{
	static_assert(TNode::use_parent, "use_parent should be true");
	assert(node1 && node2);
	if (node1 == node2) return;
	if (node1->p == node2)
	{
		if (apply_action)
			ApplyActionRootToNode(node1);
		BSTSwapChildParent(node1, node2, node2->p);
		if (update_info)
			UpdateInfoNodeToRoot(node2);
	}
	else if (node2->p == node1)
	{
		if (apply_action)
			ApplyActionRootToNode(node2);
		BSTSwapChildParent(node2, node1, node1->p);
		if (update_info)
			UpdateInfoNodeToRoot(node1);
	}
	else
	{
		if (apply_action)
		{
			ApplyActionRootToNode(node1);
			ApplyActionRootToNode(node2);
		}
		BSTSwapNotRelated(node1, node1->p, node2, node2->p);
		if (update_info)
		{
			UpdateInfoNodeToRoot(node1);
			UpdateInfoNodeToRoot(node2);
		}
	}
}
