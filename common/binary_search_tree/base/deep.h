#pragma once

namespace bst {
namespace base {
template <class TNode>
inline unsigned Deep(TNode* node) {
  unsigned d = 0;
  for (; node; node = node->p) ++d;
  return d;
}
}  // namespace base
}  // namespace bst
