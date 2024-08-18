#pragma once

#include <algorithm>

namespace bst {
namespace info {
template <class TInfo>
class Height : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = Height<TInfo>;

  static constexpr bool is_none = false;

 public:
  // Leaf node has height = 1.
  unsigned height;

 public:
  template <class TNode>
  constexpr void Update(TNode* node) {
    TBase::Update(node);
    height = 1 + std::max((node->l ? node->l->info.height : 0),
                          (node->r ? node->r->info.height : 0));
  }
};
}  // namespace info
}  // namespace bst
