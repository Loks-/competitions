#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/none.h"
#include "common/template.h"

namespace bst {
namespace info {
namespace segment {
class None : public bst::info::None {
 public:
  using TBase = bst::info::None;
  using TSelf = None;

  template <class TNode>
  void SetN(const TNode* node) {
    assert(node);
    FakeUse(node);
  }

  template <class TNode>
  void SetS(const TNode* node) {
    assert(node);
    FakeUse(node);
  }

  template <class TNode>
  void AddN(const TNode*) {}

  template <class TNode>
  void AddS(const TNode*) {}
};
}  // namespace segment
}  // namespace info
}  // namespace bst
