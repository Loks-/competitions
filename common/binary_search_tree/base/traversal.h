#pragma once

#include "../../base.h"
#include <queue>
#include <vector>

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
	static void TraversePreorder(TNode* node, std::vector<TData>& output)
	{
		if (!node) return;
		node->ApplyAction();
		output.push_back(node->data);
		TraversePreorder(node->l, output);
		TraversePreorder(node->r, output);
	}

	template<class TNode, class TData>
	static void TraverseInorder(TNode* node, std::vector<TData>& output)
	{
		if (!node) return;
		node->ApplyAction();
		TraverseInorder(node->l, output);
		output.push_back(node->data);
		TraverseInorder(node->r, output);
	}

	template<class TNode, class TData>
	static void TraversePostorder(TNode* node, std::vector<TData>& output)
	{
		if (!node) return;
		node->ApplyAction();
		TraversePostorder(node->l, output);
		TraversePostorder(node->r, output);
		output.push_back(node->data);
	}

	template<class TNode, class TData>
	static void TraverseLevelorder(TNode* node, std::vector<TData>& output)
	{
		if (!node) return;
		std::queue<TNode*> q;
		for (q.push(node); !q.empty(); q.pop())
		{
			node = q.front();
			node->ApplyAction();
			output.push_back(node->data);
			if (node->l) q.push(node->l);
			if (node->r) q.push(node->r);
		}
	}

public:
	template<class TNode, class TData>
	static std::vector<TData> Traverse(TNode* root, EBSTTraversalOrder order)
	{
		std::vector<TData> output;
		if (order == EBST_Preorder)
			TraversePreorder(root, output);
		else if (order == EBST_Inorder)
			TraverseInorder(root, output);
		else if (order == EBST_Postorder)
			TraversePostorder(root, output);
		else if (order == EBST_Levelorder)
			TraverseLevelorder(root, output);
		else
			assert(false);
		return output;
	}
};
