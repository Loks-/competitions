#pragma once

namespace {

template<class TNode>
TNode* FindByKey(TNode* root, const typename TNode::TKey& key)
{
	static_assert(TNode::use_key);
	for (TNode * node = root; node != 0; )
	{
		node->action.Apply(node);
		if (node->x < key)
			node = node->l;
		else if (key < node->x)
			node = node->r;
		else
			return node;
	}
	return 0;
}

} // namespace
