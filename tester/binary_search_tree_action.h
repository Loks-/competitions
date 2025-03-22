#pragma once

#include "common/binary_search_tree/action/add_each__sum.h"
#include "common/binary_search_tree/action/none.h"
#include "common/template.h"

template <class TNode>
inline void AddAction(TNode*, MetaType<bst::action::None>) {}

template <class TNode, class TData>
inline void AddAction(TNode* root, MetaType<bst::action::AddEachSum<TData>>) {
  if (root) root->AddAction(1);
}

template <class TNode>
inline void AddAction(TNode* root) {
  AddAction(root, MetaType<typename TNode::TAction>());
}
