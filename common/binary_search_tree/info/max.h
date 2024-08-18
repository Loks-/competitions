#pragma once

#include <algorithm>

namespace bst {
namespace info {
template <class TMaxType, class TInfo>
class Max : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = Max<TMaxType, TInfo>;

  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  TMaxType max;

 public:
  template <class TNode>
  constexpr void Update(TNode* node) {
    TBase::Update(node);
    max = node->data;
    if (node->l) max = std::max(max, node->l->info.max);
    if (node->r) max = std::max(max, node->r->info.max);
  }
};
}  // namespace info
}  // namespace bst
