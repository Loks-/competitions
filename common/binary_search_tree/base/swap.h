#pragma once

#include "sibling.h"
#include "../../base.h"
#include <utility>

template<class TNode>
inline void BSTSwapNotRelated(TNode* node1, TNode* parent1, TNode* node2, TNode* parent2)
{
	assert(node1 != node2);
	TNode* child1l = node1->l, *child1r = node1->r;
	node1->SetL(node2->l);
	node1->SetR(node2->r);
	node2->SetL(child1l);
	node2->SetR(child1r);
	if (parent1 && parent1 == parent2)
	{
		std::swap(parent1->l, parent1->r);
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
		node2->SetP(0);
	if (parent2)
	{
		if (node2 == parent2->l)
			parent2->SetL(node1);
		else
			parent2->SetR(node1);
	}
	else
		node1->SetP(0);
}

template<class TNode>
inline void BSTSwapChildParent(TNode* child, TNode* parent, TNode* gparent)
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
		child->SetP(0);
}

template<class TNode>
inline void BSTSwapAuto(TNode* node1, TNode* parent1, TNode* node2, TNode* parent2)
{
	assert(node1 && node2);
	if (node1 == node2) return;
	if (parent1 == node2)
		BSTSwapChildParent(node1, node2, parent2);
	else if (parent2 == node1)
		BSTSwapChildParent(node2, node1, parent1);
	else
		BSTSwapNotRelated(node1, parent1, node2, parent2);
}
