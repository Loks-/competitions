#pragma once

#include "../../template.h"

template<class TNode>
inline void UpdateInfoNodeToRoot(TNode* node, TFakeFalse) {}

template<class TNode>
inline void UpdateInfoNodeToRoot(TNode * node, TFakeTrue)
{
	static_assert(TNode::use_parent, "use_parent should be true");
	for (; node; node = node->p) node->UpdateInfo();
}

template<class TNode>
inline void UpdateInfoNodeToRoot(TNode* node) { UpdateInfoNodeToRoot(node, TFakeBool<!TNode::TInfo::is_none>()); }
