#pragma once

#include "../../base.h"

namespace {

template<class TNode, class TValue>
TNode* BSTPrefixSumUpperBound(TNode* root, TValue value)
{
	static_assert(TNode::TInfo::has_size, "info should contain size");
	if (!root) return 0;
	if (value >= root->info.sum) return 0;
	for (TNode* node = root; node;)
	{
		node->ApplyAction();
		TValue ls = (node->l ? node->l->info.sum : 0);
		if (value < ls)
			node = node->l;
		else if (value < ls + node->data)
			return node;
		else
		{
			value -= (ls + node->data);
			node = node->r;
		}
	}
	assert(false);
	return 0;
}

} // namespace
