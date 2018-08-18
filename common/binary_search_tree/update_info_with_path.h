#pragma once

#include "../base.h"
#include "../template.h"

namespace {

template<class TNode>
void UpdateInfoNodeToRootWithPath(const vector<TNode*>& node_to_root_path, TFakeFalse) {}

template<class TNode>
void UpdateInfoNodeToRootWithPath(const vector<TNode*>& node_to_root_path, TFakeTrue)
{
	for (auto it = node_to_root_path.begin(); it != node_to_root_path.end(); ++it)
		(*it)->UpdateInfo();
}

template<class TNode>
void UpdateInfoNodeToRootWithPath(const vector<TNode*>& node_to_root_path) { UpdateInfoNodeToRootWithPath(node_to_root_path, TFakeBool<!TNode::TInfo::is_none>()); }

} // namespace
