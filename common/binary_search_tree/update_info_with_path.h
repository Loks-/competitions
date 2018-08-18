#pragma once

#include "../base.h"
#include "../template.h"

namespace {

template<class TNode>
void UpdateInfoNodeToRootWithPath(const vector<TNode*>& node_to_root_path, unsigned start_from_index, TFakeFalse) {}

template<class TNode>
void UpdateInfoNodeToRootWithPath(const vector<TNode*>& node_to_root_path, unsigned start_from_index, TFakeTrue)
{
	for (auto it = node_to_root_path.begin() + start_from_index; it != node_to_root_path.end(); ++it)
		(*it)->UpdateInfo();
}

template<class TNode>
void UpdateInfoNodeToRootWithPath(const vector<TNode*>& node_to_root_path, unsigned start_from_index = 0)
{
	UpdateInfoNodeToRootWithPath(node_to_root_path, start_from_index, TFakeBool<!TNode::TInfo::is_none>());
}

} // namespace
