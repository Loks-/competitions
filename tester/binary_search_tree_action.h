#pragma once

#include "common/template.h"
#include "common/binary_search_tree/action.h"


template<class TNode>
void AddAction(TNode * root, TFakeType<BSTActionNone>) {}

template<class TNode>
void AddAction(TNode * root)
{
	if (!root) return;
	AddAction(root, TFakeType<typename TNode::TAction>());
}
