#pragma once

#include "action/none.h"
#include "info/segment.h"
#include "node.h"
#include "../base.h"

#include <vector>

template<
	class TTData,
	class TTInfo = STInfoSegment,
	class TTAction = STActionNone,
	bool _use_parent = true>
class SegmentTree
{
public:
	static const bool use_parent = _use_parent;

	using TData = TTData;
	using TInfo = TTInfo;
	using TAction = TTAction;
	using TSelf = SegmentTree<TData, TInfo, TAction, use_parent>;
	using TNode = STNode<TData, TInfo, TAction, use_parent>;
	using TCoordinate = typename TInfo::TCoordinate;

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
	void ResetNodes() { ResetNodes(unsigned(data.size()), unsigned(nodes.size())); }
	SegmentTree(unsigned data_size, unsigned max_nodes) { ResetNodes(data_size, max_nodes); }
	SegmentTree(unsigned data_size) { ResetNodes(data_size, 2 * data_size); }

	unsigned DataUsed() const {return used_data; }
	unsigned UsedNodes() const { return used_nodes; }
	unsigned AvailableNodes() const { return unsigned(nodes.size()) - used_nodes; }
	
	TNode* GetNewNode() { assert(used_nodes < nodes.size()); return &(nodes[used_nodes++]); }
	
protected:
	TNode* BuildTreeI(const std::vector<TData>& vdata, const std::vector<TCoordinate>& vx, unsigned first, unsigned last)
	{
		TNode* root = GetNewNode();
		if (first == last)
		{
			root->data = &(data[used_data]);
			data[used_data++] = vdata[first];
			root->info.SetCoordinate(vx[first]);
		}
		else
		{
			unsigned m = (first + last) / 2;
			root->SetL(BuildTreeI(vdata, vx, first, m));
			root->SetR(BuildTreeI(vdata, vx, m + 1, last));
		}
		root->UpdateInfo();
		return root;
	}

public:
	TNode* BuildTree(const std::vector<TData>& vdata)
	{
		// ...
		return BuildTreeI(vdata, 0, unsigned(vdata.size()));
	}

	TNode* BuildTree(const std::vector<TData>& vdata, const std::vector<TCoordinate>& vx)
	{
		assert(vdata.size() == vx.size());
		return BuildTreeI(vdata, vx, 0, unsigned(vdata.size()));
	}
};
