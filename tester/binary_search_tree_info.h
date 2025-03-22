#pragma once

#include "common/binary_search_tree/subtree_data/rbt_color.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/sum.h"

inline size_t GetInfoValueI(const bst::subtree_data::Size& info) {
  return info.size;
}

inline size_t GetInfoValueI(
    const bst::subtree_data::RBTColor<bst::subtree_data::Size>& info) {
  return info.size;
}

template <class T1, class T2>
inline size_t GetInfoValueI(const bst::subtree_data::Sum<T1, T2>& info) {
  return info.sum_value;
}

template <class T1, class T2>
inline size_t GetInfoValueI(
    const bst::subtree_data::RBTColor<bst::subtree_data::Sum<T1, T2>>& info) {
  return info.sum_value;
}

template <class TNode>
inline size_t GetInfoValue(const TNode* root) {
  return root ? GetInfoValueI(root->subtree_data) : 0;
}
