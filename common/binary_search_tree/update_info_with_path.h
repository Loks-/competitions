#pragma once

#include "../base.h"
#include "../template.h"

namespace {

template<class TNode>
void UpdateInfoNodeToRootWithPath(const vector<TNode*>& path_root_to_node, TFakeFalse) {}

template<class TNode>
void UpdateInfoNodeToRootWithPath(const vector<TNode*>& path_root_to_node, TFakeTrue)
{
	for (auto it = path_root_to_node.rbegin(); it != path_root_to_node.rend(); ++it)
		(*it)->UpdateInfo();
}

template<class TNode>
void UpdateInfoNodeToRootWithPath(const vector<TNode*>& path_root_to_node) { UpdateInfoNodeToRootWithPath(path_root_to_node, TFakeBool<!TNode::TInfo::is_none>()); }

} // namespace
