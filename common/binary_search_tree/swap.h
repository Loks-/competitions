#pragma once

#include "../base.h"
#include "sibling.h"
#include "action/apply_action.h"
#include "info/update_info.h"

namespace {

template<class TNode>
void BSTSwapINotRelated(TNode* node1, TNode* parent1, TNode* node2, TNode* parent2)
{
	assert(node1 != node2);
	TNode* child1l = node1->l, *child1r = node1->r;
	node1->SetL(node2->l);
	node1->SetR(node2->r);
	node2->SetL(child1l);
	node2->SetR(child1r);
	if (parent1 && parent1 == parent2)
	{
		swap(parent1->l, parent1->r);
		return;
	}
	if (parent1)
	{
		if (node1 == parent1->l)
			parent1->SetL(node2);
		else
			parent1->SetR(node2);
	}
	else
		node2->SetParentLink(0);
	if (parent2)
	{
		if (node2 == parent2->l)
			parent2->SetL(node1);
		else
			parent2->SetR(node1);
	}
	else
		node1->SetParentLink(0);
}

template<class TNode>
void BSTSwapIChildParent(TNode* child, TNode* parent, TNode* gparent)
{
	if (child == parent->l)
	{
		TNode* sibling = parent->r;
		parent->SetL(child->l);
		parent->SetR(child->r);
		child->SetL(parent);
		child->SetR(sibling);
	}
	else
	{
		TNode* sibling = parent->l;
		parent->SetL(child->l);
		parent->SetR(child->r);
		child->SetL(sibling);
		child->SetR(parent);
	}
	if (gparent)
	{
		if (parent == gparent->l)
			gparent->SetL(child);
		else
			gparent->SetR(child);
	}
	else
		child->SetParentLink(0);
}

template<class TNode>
void BSTSwapI(TNode* node1, TNode* parent1, TNode* node2, TNode* parent2)
{
	assert(node1 && node2);
	if (node1 == node2) return;
	if (parent1 == node2)
		BSTSwapIChildParent(node1, node2, parent2);
	else if (parent2 == node1)
		BSTSwapIChildParent(node2, node1, parent1);
	else
		BSTSwapINotRelated(node1, parent1, node2, parent2);
}

template<class TNode, bool update_info = true, bool apply_action = false>
void BSTSwap(TNode* node1, TNode* node2)
{
	static_assert(TNode::use_parent, "use_parent should be true");
	assert(node1 && node2);
	if (node1 == node2) return;
	if (node1->p == node2)
	{
		if (apply_action)
			ApplyActionRootToNode(node1);
		BSTSwapIChildParent(node1, node2, node2->p);
		if (update_info)
			UpdateInfoNodeToRoot(node2);
	}
	else if (node2->p == node1)
	{
		if (apply_action)
			ApplyActionRootToNode(node2);
		BSTSwapIChildParent(node2, node1, node1->p);
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
		BSTSwapINotRelated(node1, node1->p, node2, node2->p);
		if (update_info)
		{
			UpdateInfoNodeToRoot(node1);
			UpdateInfoNodeToRoot(node2);
		}
	}
}

} // namespace
