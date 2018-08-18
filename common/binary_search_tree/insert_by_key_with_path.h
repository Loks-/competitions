#pragma once

#include "update_info_with_path.h"

namespace {

template<class TNode>
void BSTInsertByKeyWithPath(TNode* root, TNode* node, vector<TNode*>& output_node_to_root_path)
{
	static_assert(TNode::use_key, "use_key should be true");
	output_node_to_root_path.clear();
	assert(node);
	if (!root) { output_node_to_root_path.push_back(node); return; }
	for (;;)
	{
		root->ApplyAction();
		output_node_to_root_path.push_back(root);
		if (root->key < node->key)
		{
			if (root->r)
				root = root->r;
			else
			{
				root->SetR(node);
				break;
			}
		}
		else
		{
			if (root->l)
				root = root->l;
			else
			{
				root->SetL(node);
				break;
			}
		}
	}
	output_node_to_root_path.push_back(node);
	reverse(output_node_to_root_path.begin(), output_node_to_root_path.end());
	UpdateInfoNodeToRootWithPath(output_node_to_root_path);
}

} // namespace
