#pragma once

#include "common/template.h"
#include "common/binary_search_tree/action/add.h"
#include "common/binary_search_tree/action/none.h"

template<class TNode>
inline void AddAction(TNode * root, TFakeType<BSTActionNone>) {}

template<class TNode, class TData>
inline void AddAction(TNode * root, TFakeType<BSTActionAdd<TData>>) { if (root) root->AddAction(1); }

template<class TNode>
inline void AddAction(TNode * root) { AddAction(root, TFakeType<typename TNode::TAction>()); }
