#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/at.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
template <class TNode, class TValue>
inline TValue Median(TNode* root) {
  static_assert(TNode::SubtreeDataType::has_size, "info should contain size");
  assert(root);
  const size_t size = subtree_data::size(root);
  if (size & 1)
    return TValue(base::at(root, size / 2)->key);
  else
    return TValue(base::at(root, size / 2 - 1)->key +
                  base::at(root, size / 2)->key) /
           TValue(2);
}
}  // namespace bst
