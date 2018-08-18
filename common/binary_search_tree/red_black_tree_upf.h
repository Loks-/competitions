#pragma once

//#include "action.h"
//#include "info.h"
#include "node.h"
#include "red_black_tree_base.h"
#include "rotate.h"
#include "sibling.h"
//#include "swap.h"
#include "tree.h"

template <
	class TTData,
	class TTInfo,
	class TTAction,
	class TTKey>
	class RedBlackTree<false, TTData, TTInfo, TTAction, TTKey> : public BSTree<BSTNode<TTData, RBTInfo<TTInfo>, TTAction, true, false, false, TTKey>,
	                                                                           RedBlackTree<false, TTData, TTInfo, TTAction, TTKey>>
{
public:
	static const bool use_key = true;
	static const bool use_parent = false;
	static const bool use_height = false;

	using TData = TTData;
	using TInfo = RBTInfo<TTInfo>;
	using TAction = TTAction;
	using TKey = TTKey;
	using TNode = BSTNode<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
	using TSelf = RedBlackTree<use_parent, TData, TTInfo, TAction, TKey>;
	using TTree = BSTree<TNode, TSelf>;
	friend class BSTree<TNode, TSelf>;

public:
	RedBlackTree(unsigned max_nodes) : TTree(max_nodes) {}

	static TNode* BuildTree(const vector<TNode*>& nodes)
	{
		TNode* root = 0;
		for (TNode* node : nodes)
			root = InsertByKey(root, node);
		return root;
	}

protected:
	static void SimpleRotate(TNode* child, TNode* parent)
	{
		if (parent->l == child)
		{
			parent->SetL(child->r);
			child->SetR(parent);
		}
		else
		{
			parent->SetR(child->l);
			child->SetL(parent);
		}
		parent->UpdateInfo();
		child->UpdateInfo();
	}

	static TNode* InsertByKeyI(TNode* root, TNode* node, TNode* root_parent)
	{
		if (!root) return node;
		TNode* child = 0;
		root->ApplyAction();
		bool rotate_required = false;
		if (root->key < node->key)
		{
			child = root->r = InsertByKeyI(root->r, node, root);
			rotate_required = !child->info.is_black && child->r && !child->r->info.is_black;
		}
		else
		{
			child = root->l = InsertByKeyI(root->l, node, root);
			rotate_required = !child->info.is_black && child->l && !child->l->info.is_black;
		}
		if (rotate_required)
		{
			assert(root->info.is_black);
			SimpleRotate(child, root);
			root->info.is_black = false;
			child->info.is_black = true;
			return child;
		}
		root->UpdateInfo();
		if (root->info.is_black || !child || child->info.is_black) return root;
		TNode* uncle = BSTSibling(root, root_parent);
		if (uncle && !uncle->info.is_black)
		{
			assert(root_parent->info.is_black);
			root_parent->info.is_black = false;
			root->info.is_black = true;
			uncle->info.is_black = true;
			return root;
		}
		rotate_required = ((root_parent->l == root) != (root->l == child));
		if (rotate_required)
		{
			SimpleRotate(child, root);
			return child;
		}
		return root;
	}

public:
	static TNode* InsertByKey(TNode* root, TNode* node)
	{ 
		node->info.is_black = false;
		node->UpdateInfo();
		root = InsertByKeyI(root, node, 0);
		root->info.is_black = true;
		return root;
	}
};
