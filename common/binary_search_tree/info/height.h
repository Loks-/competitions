#pragma once

#include "../info.h"

template<class TInfo>
class BSTInfoHeight : public TInfo
{
public:
	using TBase = TInfo;
	using TSelf = BSTInfoHeight<TInfo>;
	static const bool is_none = false;

	// Leaf node has height = 1.
	unsigned height;

	template<class TNode>
	void Update(TNode* node)
	{
		TBase::Update(node);
		height = 1 + max((node->l ? node->l->info.height : 0), (node->r ? node->r->info.height : 0));
	}
};
