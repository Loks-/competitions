#pragma once

#include "common/base.h"

#include <algorithm>

namespace bst {
namespace info {
template <class TInfo>
class WAVLRank : public TInfo {
 public:
  using TBase = TInfo;

 public:
  int rank = 0;

 public:
  constexpr void BTIReset() {
    TBase::BTIReset();
    rank = 0;
  }

  template <class TNode>
  constexpr void BTICopy(const TNode* node) {
    assert(node);
    TBase::BTICopy(node);
    rank = node->info.rank;
  }

  template <class TNode>
  constexpr void BTISwap(TNode* node) {
    assert(node);
    TBase::BTISwap(node);
    std::swap(rank, node->info.rank);
  }
};
}  // namespace info
}  // namespace bst
