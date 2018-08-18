#pragma once

#include "insert_by_key_with_path.h"
#include "node.h"
#include "red_black_tree_base.h"
#include "rotate.h"
#include "sibling.h"
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

	static TNode* InsertByKey(TNode* root, TNode* node)
	{
		static thread_local vector<TNode*> node_to_root_path;
		node->info.is_black = false;
		BSTInsertByKeyWithPath(root, node, node_to_root_path);
		node_to_root_path.push_back(0);
		for (unsigned node_index = 1;;)
		{
			TNode* parent = node_to_root_path[node_index++];
			if (!parent) { node->info.is_black = true; return node; }
			if (parent->info.is_black) { return root; }
			TNode* gparent = node_to_root_path[node_index++];
			TNode* uncle = BSTSibling(parent, gparent);
			if (!uncle || uncle->info.is_black)
			{
				bool rotate_required = ((gparent->l == parent) != (parent->l == node));
				if (rotate_required)
				{
					BSTRotate<TNode, false>(node, parent, gparent);
					parent = node;
				}
				TNode* ggparent = node_to_root_path[node_index++];
				BSTRotate<TNode, true>(parent, gparent, ggparent);
				gparent->info.is_black = false;
				parent->info.is_black = true;
				return ggparent ? root : parent;
			}
			gparent->info.is_black = false;
			parent->info.is_black = true;
			uncle->info.is_black = true;
			node = gparent;
		}
		assert(false);
		return 0;
	}
};
