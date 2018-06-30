#pragma once

namespace {

template<class TNode>
TNode* FindByKey(TNode* root, const typename TNode::TKey& key)
{
	static_assert(TNode::use_key);
	for (TNode * node = root; node != 0; )
	{
		node->action.Apply(node);
		if (node->key < key)
			node = node->r;
		else if (key < node->key)
			node = node->l;
		else
			return node;
	}
	return 0;
}

} // namespace
