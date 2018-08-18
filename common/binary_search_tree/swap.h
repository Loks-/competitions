#pragma once

#include "action.h"
#include "info.h"
#include "sibling.h"
#include "../base.h"

namespace {

template<class TNode>
void BSTSwapINotRelated(TNode* node1, TNode* node2)
{
	if (node1->l) node1->l->p = node2;
	if (node1->r) node1->r->p = node2;
	if (node2->l) node2->l->p = node1;
	if (node2->r) node2->r->p = node1;
	if (node1->p)
	{
		if (node1->p->l == node1)
			node1->p->l = node2;
		else
			node1->p->r = node2;
	}
	if (node2->p)
	{
		if (node2->p->l == node2)
			node2->p->l = node1;
		else
			node2->p->r = node1;
	}
	swap(node1->l, node2->l);
	swap(node1->r, node2->r);
	swap(node1->p, node2->p);
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
		if (gparent->l == parent)
			gparent->SetL(child);
		else
			gparent->SetR(child);
	}
	else
		child->SetParentLink(0);
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
		BSTSwapINotRelated(node1, node2);
		if (update_info)
		{
			UpdateInfoNodeToRoot(node1);
			UpdateInfoNodeToRoot(node2);
		}
	}
}

} // namespace
