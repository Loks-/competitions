#pragma once

#include "../base.h"
#include "node.h"

#include <utility>

template<
	class TTData,
	class TTInfo,
	class TTAction,
	bool _use_key = false,
	bool _use_parent = true,
	class TTKey = int64_t>
class SegmentTree
{
public:
	static const bool use_key = _use_key;
	static const bool use_parent = _use_parent;

	using TData = TTData;
	using TInfo = TTInfo;
	using TAction = TTAction;
	using TKey = TTKey;
	using TSelf = SegmentTree<TData, TInfo, TAction, use_key, use_parent, TKey>;
	using TNode = STNode<TData, TInfo, TAction, use_key, use_parent, TKey>;

protected:
	std::vector<TData> data;
	std::vector<TNode> nodes;
	unsigned used_data, used_nodes;

public:
	void ResetNodes(unsigned data_size, unsigned max_nodes)
	{
		data.resize(0); data.resize(data_size);
		nodes.resize(0); nodes.resize(max_nodes); 
		used_data = used_nodes = 0;
	}
	void ResetNodes() { ResetNodes(unsigned(data.size(), unsigned(nodes.size())); }
	SegmentTree(unsigned data_size, unsigned max_nodes) { ResetNodes(data_size, max_nodes); }
	SegmentTree(unsigned data_size) { ResetNodes(data_size, 2 * data_size - 1); }

	unsigned DataUsed() const {return used_data; }
	unsigned UsedNodes() const { return used_nodes; }
	unsigned AvailableNodes() const { return unsigned(nodes.size()) - used_nodes; }
	
	TNode* GetNewNode() { assert(used_nodes < nodes.size()); return &(nodes[used_nodes++]); }
	
	TNode* GetNewNode(const TData& value)
	{
		assert(used_data < data.size());
		TNode* node = GetNewNode();
		node->data = &(data[used_data]);
		data[used_data++] = value;
		node->UpdateInfo();
		return info;
	}
	
	TNode* GetNewNode(const TData& value, const TKey& key)
	{
		static_assert(use_key, "use_key should be true");
		TNode* node = GetNewNode();
		node->data = &(data[used_data]);
		data[used_data++] = value;
		node->key = key;
		node->UpdateInfo();
		return node;
	}

	TNode* GetNewNode(const std::pair<TData, TKey>& p) { return GetNewNode(p.first, p.second); }

	TNode* GetNewNode(TNode* l, TNode* r)
	{
		TNode* node = GetNewNode();
		node->SetL(l);
		node->SetR(r);
		node->CopyKey(r);
		node->UpdateInfo();
		return node;
	}

protected:
	template<class TDataKeyProxy>
	TNode* BuildTreeI(const std::vector<TDataKeyProxy>& vdata, unsigned first, unsigned last)
	{
		if (first >= last) return 0;
		if (first + 1 == last) return GetNewNode(vdata[first]);
		TNode* root = GetNewNode();
		unsigned m = (first + last) / 2;
		root->SetL(BuildTreeI(vdata, first, m));
		root->SetR(BuildTreeI(vdata, m + 1, last));
		root->CopyKey(root->r);
		root->UpdateInfo();
		return root;
	}

public:
	template<class TDataKeyProxy>
	TNode* BuildTree(const std::vector<TDataKeyProxy*>& vdata) { return BuildTreeI(vnodes, 0, unsigned(vdata.size())); }
};
