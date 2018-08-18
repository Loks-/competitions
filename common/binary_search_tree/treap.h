#pragma once

#include "node.h"
#include "tree.h"
#include "action/apply_action.h"
#include "action/none.h"
#include "info/size.h"
#include "info/update_info.h"

template <
	bool _use_key,
	bool _use_parent,
	class TTData,
	class TTInfo = BSTInfoSize,
	class TTAction = BSTActionNone,
	class TTKey = int64_t>
	class Treap : public BSTree<BSTNode<TTData, TTInfo, TTAction, _use_key, _use_parent, true, TTKey, unsigned>,
	                            Treap<_use_key, _use_parent, TTData, TTInfo, TTAction, TTKey>>
{
public:
	static const bool use_key = _use_key;
	static const bool use_parent = _use_parent;
	static const bool use_height = true;

	using TData = TTData;
	using TInfo = TTInfo;
	using TAction = TTAction;
	using TKey = TTKey;
	using TNode = BSTNode<TData, TInfo, TAction, use_key, use_parent, use_height, TKey, unsigned>;
	using TSelf = Treap<use_key, use_parent, TData, TInfo, TAction, TKey>;
	using TTree = BSTree<TNode, TSelf>;
	friend class BSTree<TNode, TSelf>;

public:
	Treap(unsigned max_nodes) : TTree(max_nodes) {}

public:
	static TNode* BuildTree(const vector<TNode*>& nodes)
	{
		if (nodes.size() == 0) return 0;
		TNode * proot = nodes[0];
		TNode * plast = proot;
		stack<TNode*> s; s.push(proot);
		for (unsigned j = 1; j < nodes.size(); ++j)
		{
			TNode* pj = nodes[j];
			if (pj->height < plast->height)
			{
				plast->SetR(pj);
				s.push(plast);
			}
			else if (pj->height >= proot->height)
			{
				for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
				pj->SetL(proot);
				proot = pj;
				s.push(proot);
			}
			else
			{
				for (plast->UpdateInfo(); pj->height >= s.top()->height; s.pop())
				{
					plast = s.top();
					plast->UpdateInfo();
				}
				pj->SetL(plast);
				s.top()->SetR(pj);
			}
			plast = pj;
		}
		for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
		return proot;
	}

protected:
	static TNode * JoinI(TNode* l, TNode* r)
	{
		if (l->height > r->height)
		{
			l->ApplyAction();
			if (!l->r)
				l->SetR(r);
			else
				l->SetR(JoinI(l->r, r));
			l->UpdateInfo();
			return l;
		}
		else
		{
			r->ApplyAction();
			if (!r->l)
				r->SetL(l);
			else
				r->SetL(JoinI(l, r->l));
			r->UpdateInfo();
			return r;
		}
	}

public:
	static TNode* Join(TNode* l, TNode* r)
	{
		if (!l) return r;
		if (!r) return l;
		return JoinI(l, r);
	}

protected:
	static void SplitByKeyI(TNode* p, const TKey& key, TNode*& output_l, TNode*& output_r)
	{
		p->ApplyAction();
		if (p->key < key)
		{
			if (p->r)
			{
				output_l = p;
				SplitByKeyI(p->r, key, p->r, output_r);
				p->UpdateInfo();
				if (p->r) p->r->SetParentLink(p);
			}
			else
			{
				output_l = p;
				output_r = 0;
			}
		}
		else
		{
			if (p->l)
			{
				output_r = p;
				SplitByKeyI(p->l, key, output_l, p->l);
				p->UpdateInfo();
				if (p->l) p->l->SetParentLink(p);
			}
			else
			{
				output_l = 0;
				output_r = p;
			}
		}
	}

public:
	static void SplitByKey(TNode* root, const TKey& key, TNode*& output_l, TNode*& output_r)
	{
		static_assert(use_key, "use_key should be true");
		if (!root)
			output_l = output_r = 0;
		else
		{
			SplitByKeyI(root, key, output_l, output_r);
			if (output_l) output_l->SetParentLink(0);
			if (output_r) output_r->SetParentLink(0);
		}
	}

protected:
	static void SplitBySizeI(TNode* p, unsigned lsize, TNode*& output_l, TNode*& output_r)
	{
		p->ApplyAction();
		unsigned hlsize = (p->l ? p->l->info.size : 0);
		if (lsize < hlsize)
		{
			output_r = p;
			SplitBySizeI(p->l, lsize, output_l, p->l);
			if (p->l) p->l->SetParentLink(p);
		}
		else if (lsize == hlsize)
		{
			output_l = p->l;
			output_r = p;
			p->l = 0;
		}
		else if (lsize == hlsize + 1)
		{
			output_l = p;
			output_r = p->r;
			p->r = 0;
		}
		else
		{
			output_l = p;
			SplitBySizeI(p->r, lsize - hlsize - 1, p->r, output_r);
			if (p->r) p->r->SetParentLink(p);
		}
		p->UpdateInfo();
	}

public:
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
			SplitBySizeI(root, lsize, output_l, output_r);
			if (output_l) output_l->SetParentLink(0);
			if (output_r) output_r->SetParentLink(0);
		}
	}

	static TNode* InsertByKey(TNode* root, TNode* node)
	{
		static_assert(use_key, "use_key should be true");
		if (!root) return node;
		root->ApplyAction();
		if (root->height >= node->height)
		{
			if (root->key < node->key)
				root->SetR(InsertByKey(root->r, node));
			else
				root->SetL(InsertByKey(root->l, node));
			root->UpdateInfo();
			return root;
		}
		else
		{
			SplitByKeyI(root, node->key, node->l, node->r);
			if (node->l) node->l->SetParentLink(node);
			if (node->r) node->r->SetParentLink(node);
			node->UpdateInfo();
			return node;
		}
	}

	static TNode* RemoveByKey(TNode* root, const TKey& key, TNode*& removed_node)
	{
		static_assert(use_key, "use_key should be true");
		if (!root) return root;
		root->ApplyAction();
		if (root->key < key)
			root->SetR(RemoveByKey(root->r, key, removed_node));
		else if (root->key > key)
			root->SetL(RemoveByKey(root->l, key, removed_node));
		else
		{
			removed_node = root;
			TNode* l = root->l; if (l) l->SetParentLink(0);
			TNode* r = root->r; if (r) r->SetParentLink(0);
			root->ResetLinks();
			root->UpdateInfo();
			return Join(l, r);
		}
		root->UpdateInfo();
		return root;
	}

	static TNode* RemoveByNode(TNode* node)
	{
		static_assert(use_parent, "use_parent should be true");
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
		UpdateInfoNodeToRoot(p);
		return Root(p);
	}
};
