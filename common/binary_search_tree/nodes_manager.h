#pragma once

#include "../base.h"
#include "../template.h"

template<class TTNode>
class BSTNodesManager
{
public:
	using TNode = TTNode;
	using TData = typename TNode::TData;
	using TKey = typename TNode::TKey;
	static const bool use_key = TNode::use_key;
	static const bool use_height = TNode::use_height;

protected:
	vector<TNode> nodes;
	unsigned first_unused_node;
	stack<TNode*> released_nodes;
	minstd_rand random_engine;

public:
	BSTNodesManager(unsigned max_nodes) : nodes(max_nodes), first_unused_node(0) {}

protected:
	void InitRandomHeightI(TNode* p, TFakeFalse) {}
	void InitRandomHeightI(TNode* p, TFakeTrue)
	{
		p->height = random_engine();
	}

public:
	void InitRandomHeight(TNode* p) { InitRandomHeightI(p, TFakeBool<use_height>()); }
	
	TNode* GetNewNode()
	{
		if (!released_nodes.empty())
		{
			TNode* p = released_nodes.top();
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
		return p;
	}

	TNode* GetNewNode(const TData& data, const TKey& key)
	{
		static_assert(use_key);
		TNode* p = GetNewNode();
		p->data = data;
		p->key = key;
		return p;
	}

	void ReleaseNode(TNode * p)
	{
		released_nodes.push(p);
	}

	unsigned UsedNodes() const { return first_unused_node - unsigned(released_nodes.size()); }
	unsigned AvailableNodes() const { return unsigned(nodes.size()) - UsedNodes(); }
};
