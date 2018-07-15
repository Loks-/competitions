#pragma once

#include "info.h"

namespace {

template<class TNode>
vector<TNode*> BSTInsertByKeyWithPath(TNode* root, TNode* node)
{
	static_assert(TNode::use_key, "use_key should be true");
	assert(node);
	vector<TNode*> path;
	for (;;)
	{
		root->ApplyAction();
		path.push_back(root);
		if (root->key < node->key)
		{
			if (root->r)
				root = root->r;
			else
			{
				root->r = node;
				break;
			}
		}
		else
		{
			if (root->l)
				root = root->l;
			else
			{
				root->l = node;
				break;
			}
		}
	}
	path.push_back(node);
	node->SetParentLink(root);
	UpdateInfoNodeToRoot(path);
	return path;
}

} // namespace
