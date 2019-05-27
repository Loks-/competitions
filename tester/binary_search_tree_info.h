#pragma once

#include "common/binary_search_tree/info/rbt_color.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/sum.h"

inline size_t GetInfoValueI(const bst::info::Size& info) { return info.size; }

inline size_t GetInfoValueI(const bst::info::RBTColor<bst::info::Size>& info) {
  return info.size;
}

template <class T1, class T2>
inline size_t GetInfoValueI(const bst::info::Sum<T1, T2>& info) {
  return info.sum;
}

template <class T1, class T2>
inline size_t GetInfoValueI(
    const bst::info::RBTColor<bst::info::Sum<T1, T2>>& info) {
  return info.sum;
}

template <class TNode>
inline size_t GetInfoValue(const TNode* root) {
  return root ? GetInfoValueI(root->info) : 0;
}
