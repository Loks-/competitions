#pragma once

#include "common/template.h"
#include "common/binary_search_tree/action.h"
#include "common/binary_search_tree/action/add.h"

namespace {

template<class TNode>
void AddAction(TNode * root, TFakeType<BSTActionNone>) {}

template<class TNode, class TData>
void AddAction(TNode * root, TFakeType<BSTActionAdd<TData>>)
{
	if (root) root->AddAction(1);
}

template<class TNode>
void AddAction(TNode * root)
{
	if (!root) return;
	AddAction(root, TFakeType<typename TNode::TAction>());
}

} // namespace
