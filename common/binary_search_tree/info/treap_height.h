#pragma once

#include "common/hash.h"

namespace bst {
namespace info {
template <class TTHeight, class TInfo>
class TreapHeight : public TInfo {
 public:
  using THeight = TTHeight;

 public:
  THeight treap_height;

 public:
  constexpr void ClearCreate(unsigned raw_index) {
    treap_height = THeight(nhash::HValue<nhash::PolicySafe>(raw_index));
  }
};
}  // namespace info
}  // namespace bst
