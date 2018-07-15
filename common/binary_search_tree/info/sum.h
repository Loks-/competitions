#pragma once

#include "../info.h"

template<class TSumType>
class BSTInfoSum : public BSTInfoSize
{
public:
	using TBase = BSTInfoSize;
	using TSelf = BSTInfoSum<TSumType>;
	static const bool use_data = true;

	TSumType sum;

	template<class TNode>
	void Update(TNode* node)
	{
		TBase::Update(node);
		sum = node->data + (node->l ? node->l->info.sum : 0) + (node->r ? node->r->info.sum : 0);
	}
};
