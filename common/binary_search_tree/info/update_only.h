#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/none.h"

namespace bst {
namespace info {
class UpdateOnly {
 public:
  using TBase = None;
  using TSelf = UpdateOnly;

  static const bool support_insert = false;
  static const bool support_remove = false;

  template <class TNode>
  void Insert(const TNode*) {
    assert(false);
  }

  template <class TNode>
  void Remove(const TNode*) {
    assert(false);
  }
};
}  // namespace info
}  // namespace bst
