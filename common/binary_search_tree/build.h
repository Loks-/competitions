#pragma once

#include "../template.h"

class BSTBuild
{
protected:
	template<class TNode>
	static void ResetParentLinksI(TNode* root, TFakeFalse) {}

	template<class TNode>
	static void ResetParentLinksI(TNode* root, TFakeTrue)
	{
		root->p = 0;
		stack<TNode*> s;
		for (s.push(root); !s.empty(); )
		{
			TNode* node = s.top(); s.pop();
			if (node->l)
			{
				node->l->p = node;
				s.push(node->l);
			}
			if (node->r)
			{
				node->r->p = node;
				s.push(node->r);
			}
		}
	}

public:
	template<class TNode>
	static void ResetParentLinks(TNode* root) { ResetParentLinksI(root, TFakeBool<TNode::use_parent>()); }

protected:
	template<class TNode>
	static TNode* BuildI(const vector<TNode*>& nodes, unsigned first, unsigned last)
	{
		if (first >= last) return 0;
		unsigned m = (first + last) / 2;
		TNode* root = nodes[m];
		root->l = BuildI(nodes, first, m);
		root->r = BuildI(nodes, m + 1, last);
		root->UpdateInfo();
		return root;
	}

public:
	template<class TNode>
	static TNode* Build(const vector<TNode*>& nodes)
	{
		TNode* root = BuildI(nodes, 0, unsigned(nodes.size()));
		ResetParentLinks(root);
		return root;
	}
};
