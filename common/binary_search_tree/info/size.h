#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/none.h"

namespace bst {
namespace info {
class Size : public None {
 public:
  using TSelf = Size;

  static constexpr bool is_none = false;
  static constexpr bool has_size = true;

 public:
  size_t size = 0;

 public:
  template <class TNode>
  constexpr void Update(TNode* node) {
    size = 1 + (node->l ? node->l->info.size : 0) +
           (node->r ? node->r->info.size : 0);
  }
};
}  // namespace info
}  // namespace bst
