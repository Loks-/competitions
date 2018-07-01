#pragma once

namespace {

template<class TNode, bool update_child = false, bool apply_action = false>
void BSTRotate(TNode* child, TNode* parent)
{
	static_assert(TNode::use_parent, "use_parent should be true");
	if (apply_action)
	{
		parent->ApplyAction();
		child->ApplyAction();
	}
	TNode* gparent = parent->p;
	if (gparent)
	{
		if (gparent->l == parent)
			gparent->l = child;
		else
			gparent->r = child;
	}
	parent->p = child;
	child->p = gparent;
	if (parent->l == child)
	{
		parent->l = child->r;
		if (parent->l) parent->l->p = parent;
		child->r = parent;
	}
	else
	{
		parent->r = child->l;
		if (parent->r) parent->r->p = parent;
		child->l = parent;
	}
	parent->UpdateInfo();
	if (update_child) child->UpdateInfo();
}

template<class TNode, bool update_child = false, bool apply_action = false>
bool MoveNodeUp(TNode* node)
{
	static_assert(TNode::use_parent, "use_parent should be true");
	if (!node || !node->p) return false;
	BSTRotate<TNode, update_child, apply_action>(node, node->p);
	return true;
}

} // namespace
