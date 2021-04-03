#pragma once

#include "common/binary_search_tree/info/max.h"

#include <algorithm>
#include <limits>

namespace bst {
namespace info {
namespace segment {
template <class TMaxType, class TInfo>
class Max : public bst::info::Max<TMaxType, TInfo> {
 public:
  using TBase = bst::info::Max<TMaxType, TInfo>;
  using TSelf = Max<TMaxType, TInfo>;

  Max() { TBase::max = std::numeric_limits<TMaxType>::min(); }

  template <class TNode>
  void SetN(const TNode* node) {
    TBase::SetN(node);
    TBase::max = node->data;
  }

  template <class TNode>
  void SetS(const TNode* node) {
    TBase::SetS(node);
    TBase::max = node->info.max;
  }

  template <class TNode>
  void AddN(const TNode* node) {
    if (node) {
      TBase::AddN(node);
      TBase::max = std::max(TBase::max, node->data);
    }
  }

  template <class TNode>
  void AddS(const TNode* node) {
    if (node) {
      TBase::AddS(node);
      TBase::max = std::max(TBase::max, node->info.max);
    }
  }
};
}  // namespace segment
}  // namespace info
}  // namespace bst
