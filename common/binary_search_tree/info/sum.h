#pragma once

namespace bst {
namespace info {
template <class TSumType, class TInfo>
class Sum : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = Sum<TSumType, TInfo>;

  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  TSumType sum;

 public:
  template <class TNode>
  constexpr void Update(TNode* node) {
    TBase::Update(node);
    sum = node->data + (node->l ? node->l->info.sum : TSumType()) +
          (node->r ? node->r->info.sum : TSumType());
  }
};
}  // namespace info
}  // namespace bst
