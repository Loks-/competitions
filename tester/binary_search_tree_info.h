#pragma once

#include "common/binary_search_tree/subtree_data/rbt_color.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/sum.h"

template <class TSubtreeInfo>
inline size_t GetInfoValueI(const TSubtreeInfo& info) {
  using TSum = bst::subtree_data::Sum<uint64_t>;
  if constexpr (TSubtreeInfo::template has<TSum>()) {
    return TSum::get(info);
  } else if constexpr (TSubtreeInfo::template has<bst::subtree_data::Size>()) {
    return bst::subtree_data::size(info);
  } else {
    return 0;
  }
}

template <class TNode>
inline size_t GetInfoValue(const TNode* root) {
  return root ? GetInfoValueI(root->subtree_data) : 0;
}
