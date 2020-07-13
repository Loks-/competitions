#pragma once

#include "common/binary_search_tree/info/none.h"

namespace bst {
namespace ext {
namespace info {
class None : public bst::info::None {
 public:
  using TSelf = None;
  static const bool support_insert = true;
  static const bool support_remove = true;

  template <class TNode>
  void Insert(const TNode*) {}

  template <class TNode>
  void Remove(const TNode*) {}
};
}  // namespace info
}  // namespace ext
}  // namespace bst
