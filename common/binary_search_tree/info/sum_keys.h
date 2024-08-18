#pragma once

namespace bst {
namespace info {
template <class TSumType, class TInfo>
class SumKeys : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = SumKeys<TSumType, TInfo>;

  static constexpr bool is_none = false;

 public:
  TSumType sum_keys;

 public:
  template <class TNode>
  constexpr void Update(TNode* node) {
    TBase::Update(node);
    sum_keys = node->key + (node->l ? node->l->info.sum_keys : 0) +
               (node->r ? node->r->info.sum_keys : 0);
  }
};
}  // namespace info
}  // namespace bst
