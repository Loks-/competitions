#pragma once

#include "common/binary_search_tree/info/size.h"

namespace bst {
namespace info {
namespace segment {
class Size : public bst::info::Size {
 public:
  using TBase = bst::info::Size;
  using TSelf = Size;

 public:
  template <class TNode>
  constexpr void SetN(const TNode* node) {
    assert(node);
    MetaUse(node);
    TBase::size = 1;
  }

  template <class TNode>
  constexpr void SetS(const TNode* node) {
    assert(node);
    TBase::size = node->info.size;
  }

  template <class TNode>
  constexpr void AddN(const TNode* node) {
    if (node) {
      TBase::size += 1;
    }
  }

  template <class TNode>
  constexpr void AddS(const TNode* node) {
    if (node) {
      TBase::size += node->info.size;
    }
  }
};
}  // namespace segment
}  // namespace info
}  // namespace bst
