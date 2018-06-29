#pragma once

#include "action.h"
#include "action_utils.h"
#include "build.h"
#include "find_by_order.h"
#include "info.h"
#include "node.h"
#include "nodes_manager.h"

template <
	bool _use_key,
	class TTData,
	class TTInfo = BSTInfoSize,
	class TTAction = BSTActionNone,
	class TTKey = int64_t>
class SplayTree : public BSTNodesManager<BSTNode<TTData, TTInfo, TTAction, _use_key, true, false, TTKey>>
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
	using TNodesManager = BSTNodesManager<TNode>;

public:
	SplayTree(unsigned max_nodes) : TNodesManager(max_nodes) {}

public:
	TNode* Build(const vector<TData>& data)
	{
		assert(TNodesManager::AvailableNodes() >= data.size());
		if (data.size() == 0) return 0;
		vector<TNode*> v(data.size());
		for (unsigned i = 0; i < data.size(); ++i)
		{
			v[i] = TNodesManager::GetNewNode(data[i]);
		}
		return BSTBuild::Build(v);
	}

	TNode* Build(const vector<TData>& data, const vector<TKey>& keys)
	{
		static_assert(use_key);
		assert(data.size() == keys.size());
		assert(TNodesManager::AvailableNodes() >= data.size());
		if (data.size() == 0) return 0;
		vector<pair<TKey, TNode*>> vp(data.size());
		for (unsigned i = 0; i < data.size(); ++i)
		{
			vp[i] = make_pair(keys[i], TNodesManager::GetNewNode(data[i], keys[i]));
		}
		sort(vp.begin(), vp.end());
		vector<TNode*> v(vp.size());
		for (unsigned i = 0; i < vp.size(); ++i)
		{
			v[i] = vp[i].second;
		}
		return BSTBuild::Build(v);
	}

protected:
	static void RotateI(TNode* child, TNode* parent)
	{
		TNode* gparent = parent->p;
		if (gparent)
		{
			if (gparent->l == parent)
				gparent->l = child;
			else
				gparent->r = child;
		}
		parent->p = child;
		child->p = gparent;
		if (parent->l == child)
		{
			parent->l = child->r;
			if (parent->l) parent->l->p = parent;
			child->r = parent;
		}
		else
		{
			parent->r = child->l;
			if (parent->r) parent->r->p = parent;
			child->l = parent;
		}
		parent->UpdateInfo();
		child->UpdateInfo();
	}

	static void SplayI(TNode* node)
	{
		for (;;)
		{
			TNode* parent = node->p;
			if (!parent) return;
			TNode* gparent = parent->p;
			if (!gparent)
			{
				RotateI(node, parent);
				return;
			}
			bool zigzig = ((gparent->l == parent) == (parent->l == node));
			if (zigzig)
			{
				RotateI(parent, gparent);
				RotateI(node, parent);
			}
			else
			{
				RotateI(node, parent);
				RotateI(node, gparent);
			}
		}
	}

public:
	static void Splay(TNode* node)
	{
		BSTActionUtils::ApplyActionRootToNode(node);
		SplayI(node);
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
		SplayI(p);
		p->r = r;
		r->p = p;
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

	static TNode* FindByKey_Less(TNode* root, const typename TNode::TKey& key)
	{
		static_assert(TNode::use_key);
		TNode * last_less = 0;
		for (TNode* node = root; node != 0; )
		{
			node->ApplyAction();
			if (node->x < key)
			{
				last_less = node;
				node = node->r;
			}
			else
				node = node->l;
		}
		return last_less;
	}

	static void SplitByKey(TNode* root, const TKey& key, TNode*& output_l, TNode*& output_r)
	{
		if (!root)
		{
			output_l = output_r = 0;
			return;
		}
		TNode* p = FindByKey_Less(root, key);
		output_l = p;
		output_r = (p ? Split(p) : root);
	}

	static void SplitBySize(TNode* root, unsigned lsize, TNode*& output_l, TNode*& output_r)
	{
		static_assert(TInfo::has_size);
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
};
