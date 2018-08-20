#pragma once

#include "../../base.h"
#include "../base/find_by_order.h"

namespace {

template<class TNode, class TValue>
TValue BSTMedian(TNode* root)
{
	static_assert(TNode::TInfo::has_size, "info should contain size");
	assert(root);
	unsigned size = root->info.size;
	if (size & 1)
		return TValue(BSTFindByOrder(root, size / 2)->key);
	else
		return TValue(BSTFindByOrder(root, size / 2 - 1)->key + BSTFindByOrder(root, size / 2)->key) / TValue(2);
}

} // namespace
