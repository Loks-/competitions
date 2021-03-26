#pragma once

#include <algorithm>

namespace bst {
namespace ext {
namespace info {
template <class TMinType, class TInfo>
class Min : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = Min<TMinType, TInfo>;
  static const bool is_none = false;
  static const bool use_data = true;
  static const bool support_remove = false;

  TMinType min;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    min = node->data;
    if (node->l) min = std::min(min, node->l->info.min);
    if (node->r) min = std::min(min, node->r->info.min);
  }

  template <class TNode>
  void Insert(const TNode* node) {
    TBase::Insert(node);
    min = std::min(min, node->info.min);
  }
};
}  // namespace info
}  // namespace ext
}  // namespace bst
