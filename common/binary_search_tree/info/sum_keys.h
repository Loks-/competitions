#pragma once

template <class TSumType, class TInfo>
class BSTInfoSumKeys : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = BSTInfoSumKeys<TSumType, TInfo>;
  static const bool is_none = false;

  TSumType sum_keys;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    sum_keys = node->key + (node->l ? node->l->info.sum_keys : 0) +
               (node->r ? node->r->info.sum_keys : 0);
  }
};
