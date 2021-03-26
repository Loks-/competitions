#pragma once

#include "common/base.h"

#include <utility>

namespace bst {
namespace info {
template <class TInfo>
class RBTColor : public TInfo {
 public:
  using TBase = TInfo;

  bool black = false;

  void BTIReset() {
    TBase::BTIReset();
    black = false;
  }

  template <class TNode>
  void BTICopy(TNode* node) {
    assert(node);
    TBase::BTICopy(node);
    black = node->info.black;
  }

  template <class TNode>
  void BTISwap(TNode* node) {
    assert(node);
    TBase::BTISwap(node);
    std::swap(black, node->info.black);
  }
};
}  // namespace info
}  // namespace bst
