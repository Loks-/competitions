#pragma once

#include "../template.h"

class BSTNodeBuild
{
protected:
	template<class TNode>
	static void ResetParentLinks(TNode* root, TFakeFalse) {}

	template<class TNode>
	static void ResetParentLinks(TNode* root, TFakeTrue)
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
	static void ResetParentLinks(TNode* root) { ResetParentLinks(root, TFakeBool<TNode::use_parent>()); }
};
