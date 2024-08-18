#pragma once

#include <algorithm>

namespace bst {
namespace info {
template <class TMinType, class TInfo>
class Min : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = Min<TMinType, TInfo>;

  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  TMinType min;

 public:
  template <class TNode>
  constexpr void Update(TNode* node) {
    TBase::Update(node);
    min = node->data;
    if (node->l) min = std::min(min, node->l->info.min);
    if (node->r) min = std::min(min, node->r->info.min);
  }
};
}  // namespace info
}  // namespace bst
