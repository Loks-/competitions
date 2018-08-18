#pragma once

#include "action.h"
#include "node.h"
#include "rotate.h"
#include "tree.h"
#include "info/size.h"

template <
	bool _use_key,
	class TTData,
	class TTInfo = BSTInfoSize,
	class TTAction = BSTActionNone,
	class TTKey = int64_t>
class SplayTree : public BSTree<BSTNode<TTData, TTInfo, TTAction, _use_key, true, false, TTKey>,
								SplayTree<_use_key, TTData, TTInfo, TTAction, TTKey>>
{
public:
	static const bool use_key = _use_key;
	static const bool use_parent = true;
	static const bool use_height = false;

	using TData = TTData;
	using TInfo = TTInfo;
	using TAction = TTAction;
	using TKey = TTKey;
	using TNode = BSTNode<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
	using TSelf = SplayTree<use_key, TData, TInfo, TAction, TKey>;
	using TTree = BSTree<TNode, TSelf>;

public:
	SplayTree(unsigned max_nodes) : TTree(max_nodes) {}

	// Splay assumes that actions are already applied from root to node.
	static void Splay(TNode* node)
	{
		if (!node) return;
		for (;;)
		{
			TNode* parent = node->p;
			if (!parent) break;
			TNode* gparent = parent->p;
			if (!gparent)
			{
				BSTRotateUp(node);
				break;
			}
			bool zigzig = ((gparent->l == parent) == (parent->l == node));
			if (zigzig)
			{
				BSTRotateUp(parent);
				BSTRotateUp(node);
			}
			else
			{
				BSTRotateUp(node);
				BSTRotateUp(node);
			}
		}
		node->UpdateInfo();
	}

	static TNode* Join(TNode* l, TNode* r)
	{
		if (!l) return r;
		if (!r) return l;
		assert(!r->p);
		TNode * p = l;
		for (; ; p = p->r)
		{
			p->ApplyAction();
			if (!p->r) break;
		}
		Splay(p);
		p->SetR(r);
		p->UpdateInfo();
		return p;
	}

	// Split tree to 2 trees.
	// p and everything left will go to left tree (and p is root).
	// everything right will go to right tree.
	static TNode* Split(TNode* p)
	{
		if (!p) return 0;
		Splay(p);
		TNode* r = p->r;
		if (r)
		{
			r->p = 0;
			p->r = 0;
			p->UpdateInfo();
		}
		return r;
	}

	static TNode* FindByKey(TNode*& root, const TKey& key)
	{
		static_assert(use_key, "use_key should be true");
		TNode* node = root, * last_node = 0;
		for (; node != 0; )
		{
			last_node = node;
			node->ApplyAction();
			if (node->key < key)
				node = node->r;
			else if (key < node->key)
				node = node->l;
			else
				break;
		}
		Splay(last_node);
		root = last_node;
		return node;
	}

	static TNode* FindByKey_Less(TNode*& root, const TKey& key)
	{
		static_assert(use_key, "use_key should be true");
		TNode * last_less = 0, * last_node = root;
		for (TNode* node = root; node != 0; )
		{
			node->ApplyAction();
			if (node->key < key)
			{
				last_less = node;
				node = node->r;
			}
			else
			{
				last_node = node;
				node = node->l;
			}
		}
		root = last_less ? last_less : last_node;
		Splay(root);
		return last_less;
	}

	static void SplitByKey(TNode* root, const TKey& key, TNode*& output_l, TNode*& output_r)
	{
		static_assert(use_key, "use_key should be true");
		if (!root)
		{
			output_l = output_r = 0;
			return;
		}
		TNode* p = FindByKey_Less(root, key);
		output_l = p;
		output_r = (p ? Split(p) : root);
	}

	static TNode* FindByOrder(TNode*& root, unsigned order_index)
	{
		static_assert(TInfo::has_size, "info should contain size");
		if (!root) return 0;
		if (order_index >= root->info.size) return 0;
		for (TNode* node = root; node;)
		{
			node->ApplyAction();
			unsigned ls = (node->l ? node->l->info.size : 0);
			if (order_index < ls)
				node = node->l;
			else if (order_index == ls)
			{
				Splay(node);
				root = node;
				return node;
			}
			else
			{
				order_index -= 1 + ls;
				node = node->r;
			}
		}
		assert(false);
		return 0;
	}

	static void SplitBySize(TNode* root, unsigned lsize, TNode*& output_l, TNode*& output_r)
	{
		static_assert(TInfo::has_size, "info should contain size");
		if (!root)
		{
			output_l = output_r = 0;
		}
		else if (lsize == 0)
		{
			output_l = 0;
			output_r = root;
		}
		else if (lsize >= root->info.size)
		{
			output_l = root;
			output_r = 0;
		}
		else
		{
			TNode* p = FindByOrder(root, lsize - 1);
			output_l = p;
			output_r = Split(p);
		}
	}

	static TNode* InsertByKey(TNode* root, TNode* node)
	{
		static_assert(use_key, "use_key should be true");
		assert(node);
		if (!root) return node;
		SplitByKey(root, node->key, node->l, node->r);
		if (node->l) node->l->p = node;
		if (node->r) node->r->p = node;
		node->UpdateInfo();
		return node;
	}

	static TNode* RemoveByNode(TNode* node)
	{
		assert(node);
		ApplyActionRootToNode(node);
		TNode* l = node->l; if (l) l->SetParentLink(0);
		TNode* r = node->r; if (r) r->SetParentLink(0);
		TNode* p = node->p;
		node->ResetLinks();
		TNode* m = Join(l, r);
		if (!p) return m;
		if (node == p->l)
			p->SetL(m);
		else
			p->SetR(m);
		Splay(p);
		return p;
	}
};
