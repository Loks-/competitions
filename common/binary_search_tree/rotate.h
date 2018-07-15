#pragma once

namespace {

template<class TNode, bool update_child = false, bool apply_action = false>
void BSTRotate(TNode* child, TNode* parent, TNode* gparent)
{
	assert(child && parent);
	if (apply_action)
	{
		parent->ApplyAction();
		child->ApplyAction();
	}
	if (gparent)
	{
		if (gparent->l == parent)
			gparent->l = child;
		else
			gparent->r = child;
	}
	parent->SetParentLink(child);
	child->SetParentLink(gparent);
	if (parent->l == child)
	{
		parent->l = child->r;
		if (parent->l) parent->l->SetParentLink(parent);
		child->r = parent;
	}
	else
	{
		parent->r = child->l;
		if (parent->r) parent->r->SetParentLink(parent);
		child->l = parent;
	}
	parent->UpdateInfo();
	if (update_child) child->UpdateInfo();
}

template<class TNode, bool update_child = false, bool apply_action = false>
bool BSTRotateUp(TNode* node)
{
	static_assert(TNode::use_parent, "use_parent should be true");
	if (!node || !node->p) return false;
	BSTRotate<TNode, update_child, apply_action>(node, node->p, node->p->p);
	return true;
}

} // namespace
