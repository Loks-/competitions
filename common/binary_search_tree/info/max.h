#pragma once

#include <algorithm>

template<class TMaxType, class TInfo>
class BSTInfoMax : public TInfo
{
public:
	using TBase = TInfo;
	using TSelf = BSTInfoMax<TMaxType, TInfo>;
	static const bool is_none = false;
	static const bool use_data = true;

	TMaxType subtree_max;

	template<class TNode>
	void Update(TNode* node)
	{
		TBase::Update(node);
		subtree_max = node->data;
		if (node->l)
			subtree_max = std::max(subtree_max, node->l->info.subtree_max);
		if (node->r)
			subtree_max = std::max(subtree_max, node->r->info.subtree_max);
	}
};
