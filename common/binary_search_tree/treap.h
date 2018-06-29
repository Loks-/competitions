#pragma once

#include "action.h"
#include "build.h"
#include "info.h"
#include "node.h"
#include "nodes_manager.h"
#include "../template.h"

template <
	bool _use_key,
	bool _use_parent,
	class TTData,
	class TTInfo = BSTInfoSize,
	class TTAction = BSTActionNone,
	class TTKey = int64_t>
class Treap : public BSTNodesManager<BSTNode<TTData, TTInfo, TTAction, _use_key, _use_parent, true, TTKey, unsigned>>
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
	using TNodesManager = BSTNodesManager<TNode>;

public:
	Treap(unsigned max_nodes) : TNodesManager(max_nodes) {}

public:
	TNode* Build(const vector<TData>& data)
	{
		assert(TNodesManager::AvailableNodes() >= data.size());
		if (data.size() == 0) return 0;
		TNode * proot = TNodesManager::GetNewNode(data[0]);
		TNode * plast = proot;
		stack<TNode*> s; s.push(proot);
		for (unsigned j = 1; j < data.size(); ++j)
		{
			TNode* pj = TNodesManager::GetNewNode(data[j]);
			if (pj->height < plast->height)
			{
				plast->r = pj;
				s.push(plast);
			}
			else if (pj->height >= proot->height)
			{
				for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
				pj->l = proot;
				proot = pj;
				s.push(proot);
			}
			else
			{
				for (plast->UpdateInfo(); pj->height>= s.top()->height; s.pop())
				{
					plast = s.top();
					plast->UpdateInfo();
				}
				pj->l = plast;
				s.top()->r = pj;
			}
			plast = pj;
		}		
		for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
		BSTBuild::ResetParentLinks(proot);
		return proot;
	}

	TNode* Build(const vector<TData>& data, const vector<TKey>& keys)
	{
		static_assert(use_key);
		assert(data.size() == keys.size());
		assert(TNodesManager::AvailableNodes() >= data.size());
		if (data.size() == 0) return 0;
		vector<pair<TKey, TNode*>> vp;
		vp.reserve(data.size());
		for (unsigned j = 0; j < data.size(); ++j)
		{
			TNode * pj = TNodesManager::GetNewNode(data[j], keys[j]);
			vp.push_back(make_pair(pj->key, pj));
		}
		sort(vp.begin(), vp.end());
		TNode * proot = vp[0].second;
		TNode * plast = proot;
		stack<TNode*> s; s.push(proot);
		for (unsigned j = 1; j < data.size(); ++j)
		{
			TNode* pj = vp[j].second;
			if (pj->height < plast->height)
			{
				plast->r = pj;
				s.push(plast);
			}
			else if (pj->height >= proot->height)
			{
				for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
				pj->l = proot;
				proot = pj;
				s.push(proot);
			}
			else
			{
				for (plast->UpdateInfo(); pj->heigh >= s.top()->height; s.pop())
				{
					plast = s.top();
					plast->UpdateInfo();
				}
				pj->l = plast;
				s.top()->r = pj;
			}
			plast = pj;
		}
		for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
		BSTBuild::ResetParentLinks(proot, TFakeBool<use_parent>());
		return proot;
	}

protected:
	static TNode * JoinIT(TNode* l, TNode* r)
	{
		if (l->height > r->height)
		{
			l->ApplyAction();
			if (!l->r)
			{
				l->r = r;
				r->p = l;
			}
			else
			{
				l->r = JoinIT(l->r, r);
				l->r->p = l;
			}
			l->UpdateInfo();
			return l;
		}
		else
		{
			r->ApplyAction();
			if (!r->l)
			{
				r->l = l;
				l->p = r;
			}
			else
			{
				r->l = JoinIT(l, r->l);
				r->l->p = r;
			}
			r->UpdateInfo();
			return r;
		}
	}

	static TNode* JoinIF(TNode* l, TNode* r)
	{
		if (l->height > r->height)
		{
			l->ApplyAction();
			if (!l->r)
				l->r = r;
			else
				l->r = JoinIF(l->r, r);
			l->UpdateInfo();
			return l;
		}
		else
		{
			r->ApplyAction();
			if (!r->l)
				r->l = l;
			else
				r->l = JoinIF(l, r->l);
			r->UpdateInfo();
			return r;
		}
	}

	static TNode* JoinI(TNode* l, TNode* r, TFakeTrue) { return JoinIT(l, r); }
	static TNode* JoinI(TNode* l, TNode* r, TFakeFalse) { return JoinIF(l, r); }

public:
	static TNode* Join(TNode* l, TNode* r)
	{
		if (!l) return r;
		if (!r) return l;
		return JoinI(l, r, TFakeBool<use_parent>());
	}

protected:
	static void SplitByKeyIT(TNode* p, const TKey& key, TNode*& output_l, TNode*& output_r)
	{
		p->ApplyAction();
		if (p->key < key)
		{
			if (p->r)
			{
				output_l = p;
				SplitByKeyIT(p->r, key, p->r, output_r);
				p->UpdateInfo();
				if (p->r) p->r->p = p;
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
				SplitByKeyIT(p->l, key, output_l, p->l);
				p->UpdateInfo();
				if (p->l) p->l->p = p;
			}
			else
			{
				output_l = 0;
				output_r = p;
			}
		}
	}

	static void SplitByKeyIF(TNode* p, const TKey& key, TNode*& output_l, TNode*& output_r)
	{
		p->ApplyAction();
		if (p->key < key)
		{
			if (p->r)
			{
				output_l = p;
				SplitByKeyIF(p->r, key, p->r, output_r);
				p->UpdateInfo();
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
				SplitByKeyIF(p->l, key, output_l, p->l);
				p->UpdateInfo();
			}
			else
			{
				output_l = 0;
				output_r = p;
			}
		}
	}

	static void SplitByKeyI(TNode* p, const TKey& key, TNode*& output_l, TNode*& output_r, TFakeTrue)
	{
		SplitByKeyIT(p, key, output_l, output_r);
		if (output_l) output_l->p = 0;
		if (output_r) output_r->p = 0;
	}
	static void SplitByKeyI(TNode* p, const TKey& key, TNode*& output_l, TNode*& output_r, TFakeFalse) { SplitByKeyIF(p, key, output_l, output_r); }

public:
	static void SplitByKey(TNode* root, const TKey& key, TNode*& output_l, TNode*& output_r)
	{
		if (!root)
			output_l = output_r = 0;
		else
			SplitByKeyI(root, key, output_l, output_r, TFakeBool<use_parent>());
	}

protected:
	static void SplitBySizeIT(TNode* p, unsigned lsize, TNode*& output_l, TNode*& output_r)
	{
		p->ApplyAction();
		unsigned hlsize = (p->l ? p->l->info.size : 0);
		if (lsize < hlsize)
		{
			output_r = p;
			SplitBySizeIT(p->l, lsize, output_l, p->l);
			p->l->p = p;
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
			SplitBySizeIT(p->r, lsize - hlsize - 1, p->r, output_r);
			p->r->p = p;
		}
		p->UpdateInfo();
	}

	static void SplitBySizeIF(TNode* p, unsigned lsize, TNode*& output_l, TNode*& output_r)
	{
		p->ApplyAction();
		unsigned hlsize = (p->l ? p->l->info.size : 0);
		if (lsize < hlsize)
		{
			output_r = p;
			SplitBySizeIF(p->l, lsize, output_l, p->l);
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
			SplitBySizeIF(p->r, lsize - hlsize - 1, p->r, output_r);
		}
		p->UpdateInfo();
	}

	static void SplitBySizeI(TNode* p, unsigned lsize, TNode*& output_l, TNode*& output_r, TFakeTrue)
	{
		SplitBySizeIT(p, lsize, output_l, output_r);
		output_l->p = 0;
		output_r->p = 0;
	}
	static void SplitBySizeI(TNode* p, unsigned lsize, TNode*& output_l, TNode*& output_r, TFakeFalse) { SplitBySizeIF(p, lsize, output_l, output_r); }

public:
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
			SplitBySizeI(root, lsize, output_l, output_r, TFakeBool<use_parent>());
		}
	}

	static TNode* Insert(TNode* root, TNode* node)
	{
		assert(node);
		if (!root) return node;		
		TNode *p1, *p2;
		SplitByKey(root, node->key, p1, p2);
		return Join(Join(p1, node), p2);
	}

	TNode* Insert(TNode* root, const TData& data, const TKey& key)
	{
		return Insert(root, TNodesManager::GetNewNode(data, key));
	}

	static TNode* Remove(TNode* node)
	{
		static_assert(use_parent);
		assert(node);
		ApplyActionRootToNode(node);
		TNode* l = node->l; node->l = 0;
		if (l) l->p = 0;
		TNode* r = node->r; node->r = 0;
		if (r) r->p = 0;
		TNode* m = Join(l, r);
		TNode* p = node->p; node->p = 0;
		if (!p) return m;
		if (node == p->l)
			p->l = m;
		else
			p->r = m;
		UpdateInfoNodeToRoot(p);
		return Root(p);
	}

	TNode* RemoveAndRelease(TNode* node)
	{
		TNode* new_root = Remove(node);
		TNodesManager::ReleaseNode(node);
		return new_root;
	}
};
