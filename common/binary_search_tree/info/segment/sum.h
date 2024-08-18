#pragma once

#include "common/binary_search_tree/info/sum.h"

namespace bst {
namespace info {
namespace segment {
template <class TSumType, class TInfo>
class Sum : public bst::info::Sum<TSumType, TInfo> {
 public:
  using TBase = bst::info::Sum<TSumType, TInfo>;
  using TSelf = Sum<TSumType, TInfo>;

 public:
  constexpr Sum() { TBase::sum = TSumType(0); }

  template <class TNode>
  constexpr void SetN(const TNode* node) {
    TBase::SetN(node);
    TBase::sum = node->data;
  }

  template <class TNode>
  constexpr void SetS(const TNode* node) {
    TBase::SetS(node);
    TBase::sum = node->info.sum;
  }

  template <class TNode>
  constexpr void AddN(const TNode* node) {
    TBase::AddN(node);
    if (node) {
      TBase::sum += node->data;
    }
  }

  template <class TNode>
  constexpr void AddS(const TNode* node) {
    TBase::AddS(node);
    if (node) {
      TBase::sum += node->info.sum;
    }
  }
};
}  // namespace segment
}  // namespace info
}  // namespace bst
