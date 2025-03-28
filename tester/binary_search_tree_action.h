#pragma once

#include "common/binary_search_tree/deferred/add_each.h"
#include "common/template.h"

template <class TNode, typename TData>
inline void AddAction(TNode*, MetaTrue) {}

template <class TNode, typename TData>
inline void AddAction(TNode* root, MetaFalse) {
  if (root) bst::deferred::add_to_each<TNode, TData>(root, 1);
}

template <class TNode, typename TData>
inline void AddAction(TNode* root) {
  AddAction<TNode, TData>(root, MetaBool<TNode::DeferredType::empty>{});
}
