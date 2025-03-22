#pragma once

namespace bst {
namespace ext {
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
    sum = node->data + (node->l ? node->l->subtree_data.sum : TSumType(0)) +
          (node->r ? node->r->subtree_data.sum : TSumType(0));
  }

  template <class TNode>
  void Insert(const TNode* node) {
    TBase::Insert(node);
    sum += node->subtree_data.sum;
  }

  template <class TNode>
  void Remove(const TNode* node) {
    TBase::Remove(node);
    sum -= node->subtree_data.sum;
  }
};
}  // namespace info
}  // namespace ext
}  // namespace bst
