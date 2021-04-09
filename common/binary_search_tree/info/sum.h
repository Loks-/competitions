#pragma once

namespace bst {
namespace info {
template <class TSumType, class TInfo>
class Sum : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = Sum<TSumType, TInfo>;
  static const bool is_none = false;
  static const bool use_data = true;

  TSumType sum;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    sum = node->data + (node->l ? node->l->info.sum : TSumType()) +
          (node->r ? node->r->info.sum : TSumType());
  }
};
}  // namespace info
}  // namespace bst
