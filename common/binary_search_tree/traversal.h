#pragma once

#include "../base.h"

enum EBSTTraversalOrder
{
	EBST_Preorder,
	EBST_Inorder,
	EBST_Postorder,
	EBST_Levelorder
};

class BSTTraversal
{
protected:
	template<class TNode, class TData>
	static void TraversePreorder(TNode* node, vector<TData>& output)
	{
		if (node)
		{
			output.push_back(node->data);
			TraversePreorder(node->l, output);
			TraversePreorder(node->r, output);
		}
	}

	template<class TNode, class TData>
	static void TraverseInorder(TNode* node, vector<TData>& output)
	{
		if (node)
		{
			TraverseInorder(node->l, output);
			output.push_back(node->data);
			TraverseInorder(node->r, output);
		}
	}

	template<class TNode, class TData>
	static void TraversePostorder(TNode* node, vector<TData>& output)
	{
		if (node)
		{
			TraversePostorder(node->l, output);
			TraversePostorder(node->r, output);
			output.push_back(node->data);
		}
	}

	template<class TNode, class TData>
	static void TraverseLevelorder(TNode* node, vector<TData>& output)
	{
		queue<TNode*> q;
		for (q.push(node); !q.empty(); q.pop())
		{
			node = q.front();
			if (node)
			{
				output.push_back(node->data);
				q.push(node->l);
				q.push(node->r);
			}
		}
	}

public:
	template<class TNode, class TData>
	static vector<TData> Traverse(TNode* root, EBSTTraversalOrder order)
	{
		vector<TData> output;
		if (order == EBST_Preorder)
		{
			TraversePreorder(root, output);
		}
		else if (order == EBST_Inorder)
		{
			TraverseInorder(root, output);
		}
		else if (order == EBST_Postorder)
		{
			TraversePostorder(root, output);
		}
		else if (order == EBST_Levelorder)
		{
			TraverseLevelorder(root, output);
		}
		else
		{
			assert(false);
		}
		return output;
	}
};
