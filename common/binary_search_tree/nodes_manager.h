#pragma once

#include "build.h"
#include "../base.h"
#include "../template.h"

template<class TTNode, class TTMe>
class BSTNodesManager
{
public:
	using TNode = TTNode;
	using TData = typename TNode::TData;
	using TKey = typename TNode::TKey;
	using TMe = TTMe;
	static const bool use_key = TNode::use_key;
	static const bool use_height = TNode::use_height;

protected:
	vector<TNode> nodes;
	unsigned first_unused_node;
	stack<TNode*> released_nodes;
	minstd_rand random_engine;

public:
	BSTNodesManager(unsigned max_nodes) : nodes(max_nodes), first_unused_node(0) {}
	TMe* Me() { return static_cast<TMe*>(this); }
	const TMe* Me() const { return static_cast<const TMe*>(this); }

protected:
	void InitRandomHeightI(TNode* p, TFakeFalse) {}
	void InitRandomHeightI(TNode* p, TFakeTrue)	{ p->height = random_engine(); }

public:
	void InitRandomHeight(TNode* p) { InitRandomHeightI(p, TFakeBool<use_height>()); }
	
	TNode* GetNewNode()
	{
		if (!released_nodes.empty())
		{
			TNode* p = released_nodes.top();
			p->ResetLinks();
			released_nodes.pop();
			return p;
		}
		else
		{
			TNode * p = &(nodes[first_unused_node++]);
			InitRandomHeight(p);
			return p;
		}
	}

	TNode* GetNewNode(const TData& data)
	{
		TNode* p = GetNewNode();
		p->data = data;
		p->UpdateInfo();
		return p;
	}

	TNode* GetNewNode(const TData& data, const TKey& key)
	{
		static_assert(use_key);
		TNode* p = GetNewNode();
		p->data = data;
		p->key = key;
		p->UpdateInfo();
		return p;
	}

	void ReleaseNode(TNode * p) { released_nodes.push(p); }
	unsigned UsedNodes() const { return first_unused_node - unsigned(released_nodes.size()); }
	unsigned AvailableNodes() const { return unsigned(nodes.size()) - UsedNodes(); }

	TNode* Build(const vector<TData>& data)
	{
		assert(AvailableNodes() >= data.size());
		if (data.size() == 0) return 0;
		vector<TNode*> v(data.size());
		for (unsigned i = 0; i < data.size(); ++i)
			v[i] = GetNewNode(data[i]);
		return TMe::BuildTree(v);
	}

	TNode* Build(const vector<TData>& data, const vector<TKey>& keys)
	{
		static_assert(use_key);
		assert(data.size() == keys.size());
		assert(AvailableNodes() >= data.size());
		if (data.size() == 0) return 0;
		vector<pair<TKey, TNode*>> vp(data.size());
		for (unsigned i = 0; i < data.size(); ++i)
			vp[i] = make_pair(keys[i], GetNewNode(data[i], keys[i]));
		sort(vp.begin(), vp.end());
		vector<TNode*> v(vp.size());
		for (unsigned i = 0; i < vp.size(); ++i)
			v[i] = vp[i].second;
		return TMe::BuildTree(v);
	}

	static TNode* BuildTree(const vector<TNode*>& vnodes) { return BSTBuild::Build(vnodes); }
	TNode* InsertNewNode(TNode* root, const TData& data, const TKey& key) { return TMe::Insert(root, GetNewNode(data, key)); }

	TNode* RemoveAndRelease(TNode* node)
	{
		TNode* new_root = TMe::Remove(node);
		ReleaseNode(node);
		return new_root;
	}
};
