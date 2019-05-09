#pragma once

#include <algorithm>

template <class TMinType, class TInfo>
class BSTInfoMin : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = BSTInfoMin<TMinType, TInfo>;
  static const bool is_none = false;
  static const bool use_data = true;

  TMinType subtree_min;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    subtree_min = node->data;
    if (node->l) subtree_min = std::min(subtree_min, node->l->info.subtree_min);
    if (node->r) subtree_min = std::min(subtree_min, node->r->info.subtree_min);
  }
};
