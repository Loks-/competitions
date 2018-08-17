#pragma once

#include "info.h"
namespace {

template<class TNode>
void UpdateInfoNodeToRoot(const vector<TNode*>& path_root_to_node, TFakeFalse) {}

template<class TNode>
void UpdateInfoNodeToRoot(const vector<TNode*>& path_root_to_node, TFakeTrue)
{
	for (auto it = path_root_to_node.rbegin(); it != path_root_to_node.rend(); ++it)
		(*it)->UpdateInfo();
}

template<class TNode>
void UpdateInfoNodeToRoot(const vector<TNode*>& path_root_to_node) { UpdateInfoNodeToRoot(path_root_to_node, TFakeBool<!TNode::TInfo::is_none>()); }

} // namespace
