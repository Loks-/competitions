#pragma once

namespace bst {
namespace {
namespace info {
template <class TSumType, class TInfo>
class SumKeys : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = SumKeys<TSumType, TInfo>;
  static const bool is_none = false;

  TSumType sum_keys;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    sum_keys = node->key + (node->l ? node->l->info.sum_keys : 0) +
               (node->r ? node->r->info.sum_keys : 0);
  }

  template <class TNode>
  void Insert(const TNode* node) {
    TBase::Insert(node);
    sum_keys += node->info.sum_keys;
  }

  template <class TNode>
  void Remove(const TNode* node) {
    TBase::Remove(node);
    sum_keys -= node->info.sum_keys;
  }
};
}  // namespace info
}  // namespace
}  // namespace bst
