#pragma once

#include "common/binary_search_tree/info/sum.h"

#include <algorithm>

namespace bst {
namespace info {
namespace segment {
template <class TSumType, class TInfo>
class Sum : public bst::info::Sum<TSumType, TInfo> {
 public:
  using TBase = bst::info::Sum<TSumType, TInfo>;
  using TSelf = Sum<TSumType, TInfo>;

  Sum() { TBase::sum = TSumType(0); }

  template <class TNode>
  void SetN(const TNode* node) {
    TBase::SetN(node);
    TBase::sum = node->data;
  }

  template <class TNode>
  void SetS(const TNode* node) {
    TBase::SetS(node);
    TBase::sum = node->info.sum;
  }

  template <class TNode>
  void AddN(const TNode* node) {
    if (node) {
      TBase::AddN(node);
      TBase::sum += node->data;
    }
  }

  template <class TNode>
  void AddS(const TNode* node) {
    if (node) {
      TBase::AddS(node);
      TBase::sum += node->info.sum;
    }
  }
};
}  // namespace segment
}  // namespace info
}  // namespace bst
