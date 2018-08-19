#pragma once

#include "node.h"
#include "tree.h"
#include "action/none.h"
#include "base/rotate.h"
#include "info/height.h"
#include "info/size.h"

template <
	bool _use_parent,
	class TTData,
	class TTInfo = BSTInfoSize,
	class TTAction = BSTActionNone,
	class TTKey = int64_t>
class AVLTree : public BSTree<BSTNode<TTData, BSTInfoHeight<TTInfo>, TTAction, true, _use_parent, false, TTKey>,
	                          AVLTree<_use_parent, TTData, TTInfo, TTAction, TTKey>>
{
public:
	static const bool use_key = true;
	static const bool use_parent = _use_parent;
	static const bool use_height = false;

	using TData = TTData;
	using TInfo = BSTInfoHeight<TTInfo>;
	using TAction = TTAction;
	using TKey = TTKey;
	using TNode = BSTNode<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
	using TSelf = AVLTree<use_parent, TData, TTInfo, TAction, TKey>;
	using TTree = BSTree<TNode, TSelf>;
	friend class BSTree<TNode, TSelf>;

public:
	AVLTree(unsigned max_nodes) : TTree(max_nodes) {}

	static int Height(TNode* node) { return node ? int(node->info.height) : 0; }
	static int Balance(TNode* node) { return node ? Height(node->l) - Height(node->r) : 0; }

	static TNode* InsertByKey(TNode* root, TNode* node)
	{
		if (!root) return node;
		root->ApplyAction();
		if (root->key < node->key)
			root->SetR(InsertByKey(root->r, node));
		else
			root->SetL(InsertByKey(root->l, node));
		if (Balance(root) == 2)
		{
			if (Balance(root->l) == -1)
				BSTRotate<TNode, false, false>(root->l->r, root->l, root);
			TNode* child = root->l;
			BSTRotate<TNode, true, false>(child, root, 0);
			return child;
		}
		else if (Balance(root) == -2)
		{
			if (Balance(root->r) == 1)
				BSTRotate<TNode, false, false>(root->r->l, root->r, root);
			TNode* child = root->r;
			BSTRotate<TNode, true, false>(child, root, 0);
			return child;
		}
		root->UpdateInfo();
		return root;
	}
};
