#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/none.h"

namespace bst {
namespace info {
namespace segment {
class None : public bst::info::None {
 public:
  using TBase = bst::info::None;
  using TSelf = None;

 public:
  template <class TNode>
  constexpr void SetN([[maybe_unused]] const TNode* node) {
    assert(node);
  }

  template <class TNode>
  constexpr void SetS([[maybe_unused]] const TNode* node) {
    assert(node);
  }

  template <class TNode>
  constexpr void AddN(const TNode*) {}

  template <class TNode>
  constexpr void AddS(const TNode*) {}
};
}  // namespace segment
}  // namespace info
}  // namespace bst
