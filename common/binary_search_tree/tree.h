#pragma once

#include "../base.h"
#include "nodes_manager.h"
#include "base/find_by_key.h"
#include "base/find_by_order.h"
#include "../template.h"
#include <algorithm>
#include <vector>
#include <utility>

template<class TTNode, class TTMe>
class BSTree : public BSTNodesManager<TTNode>
{
public:
	using TNode = TTNode;
	using TData = typename TNode::TData;
	using TKey = typename TNode::TKey;
	using TMe = TTMe;
	using TNodesManager = BSTNodesManager<TNode>;
	
	static const bool use_key = TNode::use_key;
	static const bool use_parent = TNode::use_parent;

public:
	BSTree(unsigned max_nodes) : TNodesManager(max_nodes) {}
	TMe* Me() { return static_cast<TMe*>(this); }
	const TMe* Me() const { return static_cast<const TMe*>(this); }

	static TNode* BuildTreeI(const std::vector<TNode*>& vnodes, unsigned first, unsigned last)
	{
		if (first >= last) return 0;
		unsigned m = (first + last) / 2;
		TNode* root = vnodes[m];
		root->SetL(BuildTreeI(vnodes, first, m));
		root->SetR(BuildTreeI(vnodes, m + 1, last));
		root->UpdateInfo();
		return root;
	}

	static TNode* BuildTree(const std::vector<TNode*>& vnodes) { return BuildTreeI(vnodes, 0, unsigned(vnodes.size())); }

	TNode* Build(const std::vector<TData>& data)
	{
		assert(TNodesManager::AvailableNodes() >= data.size());
		if (data.size() == 0) return 0;
		std::vector<TNode*> v(data.size());
		for (unsigned i = 0; i < data.size(); ++i)
			v[i] = TNodesManager::GetNewNode(data[i]);
		return TMe::BuildTree(v);
	}

	TNode* Build(const std::vector<TData>& data, const std::vector<TKey>& keys)
	{
		static_assert(use_key, "use_key should be true");
		assert(data.size() == keys.size());
		assert(TNodesManager::AvailableNodes() >= data.size());
		if (data.size() == 0) return 0;
		std::vector<std::pair<TKey, TNode*>> vp(data.size());
		for (unsigned i = 0; i < data.size(); ++i)
			vp[i] = std::make_pair(keys[i], TNodesManager::GetNewNode(data[i], keys[i]));
		std::sort(vp.begin(), vp.end());
		std::vector<TNode*> v(vp.size());
		for (unsigned i = 0; i < vp.size(); ++i)
			v[i] = vp[i].second;
		return TMe::BuildTree(v);
	}

	static TNode* FindByKey(TNode* root, const typename TNode::TKey& key) { return BSTFindByKey(root, key); }
	static TNode* FindByOrder(TNode* root, unsigned order_index) { return BSTFindByOrder(root, order_index); }

	static TNode* InsertByKey(TNode* root, TNode* node)
	{
		static_assert(use_key, "use_key should be true");
		assert(node);
		if (!root) return node;
		return TMe::InsertByKeyI(root, node, TFakeBool<use_parent>());
	}

	TNode* InsertNewNode(TNode* root, const TData& data, const TKey& key) { return TMe::InsertByKey(root, TNodesManager::GetNewNode(data, key)); }

protected:
	static TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node, TFakeTrue)
	{
		removed_node = TMe::FindByKey(root, key);
		return (removed_node ? TMe::RemoveByNode(removed_node) : root);
	}

	static TNode* RemoveByOrderI(TNode* root, unsigned order_index, TNode*& removed_node, TFakeTrue)
	{
		removed_node = TMe::FindByOrder(root, order_index);
		return (removed_node ? TMe::RemoveByNode(removed_node) : root);
	}

public:
	static TNode* RemoveByKey(TNode* root, const TKey& key, TNode*& removed_node) { return TMe::RemoveByKeyI(root, key, removed_node, TFakeBool<use_parent>()); }
	static TNode* RemoveByOrder(TNode* root, unsigned order_index, TNode*& removed_node) { return TMe::RemoveByOrderI(root, order_index, removed_node, TFakeBool<use_parent>()); }

	TNode* RemoveAndReleaseByNode(TNode* node)
	{
		TNode* new_root = TMe::RemoveByNode(node);
		TNodesManager::ReleaseNode(node);
		return new_root;
	}

	TNode* RemoveAndReleaseByKey(TNode* root, const TKey& key)
	{
		TNode* removed_node = 0, *new_root = TMe::RemoveByKey(root, key, removed_node);
		if (removed_node) TNodesManager::ReleaseNode(removed_node);
		return new_root;
	}

	TNode* RemoveAndReleaseByOrder(TNode* root, unsigned order_index)
	{
		TNode* removed_node = 0, * new_root = TMe::RemoveByOrder(root, order_index, removed_node);
		if (removed_node) TNodesManager::ReleaseNode(removed_node);
		return new_root;
	}

	void ReleaseTree(TNode* root)
	{
		if (root)
		{
			ReleaseTree(root->l);
			ReleaseTree(root->r);
			TNodesManager::ReleaseNode(root);
		}
	}
};
