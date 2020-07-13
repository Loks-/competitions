#pragma once

#include <algorithm>

namespace bst {
namespace ext {
namespace info {
template <class TMaxType, class TInfo>
class Max : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = Max<TMaxType, TInfo>;
  static const bool is_none = false;
  static const bool use_data = true;
  static const bool support_remove = false;

  TMaxType max;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    max = node->data;
    if (node->l) max = std::max(max, node->l->info.max);
    if (node->r) max = std::max(max, node->r->info.max);
  }

  template <class TNode>
  void Insert(const TNode* node) {
    TBase::Insert(node);
    max = std::max(max, node->info.max);
  }
};
}  // namespace info
}  // namespace ext
}  // namespace bst
