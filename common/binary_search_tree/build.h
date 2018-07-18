#pragma once

namespace {

template<class TNode>
static TNode* BSTBuild(const vector<TNode*>& nodes, unsigned first, unsigned last)
{
	if (first >= last) return 0;
	unsigned m = (first + last) / 2;
	TNode* root = nodes[m];
	root->SetL(BSTBuild(nodes, first, m));
	root->SetR(BSTBuild(nodes, m + 1, last));
	root->UpdateInfo();
	return root;
}

template<class TNode>
static TNode* BSTBuild(const vector<TNode*>& nodes)
{
	return BSTBuild(nodes, 0, unsigned(nodes.size()));
}

} // namespace
