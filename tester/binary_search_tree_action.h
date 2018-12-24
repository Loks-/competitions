#pragma once

#include "common/binary_search_tree/action/add.h"
#include "common/binary_search_tree/action/none.h"
#include "common/template.h"

template <class TNode>
inline void AddAction(TNode* root, TFakeType<BSTActionNone>) {}

template <class TNode, class TData>
inline void AddAction(TNode* root, TFakeType<BSTActionAdd<TData>>) {
  if (root) root->AddAction(1);
}

template <class TNode>
inline void AddAction(TNode* root) {
  AddAction(root, TFakeType<typename TNode::TAction>());
}
