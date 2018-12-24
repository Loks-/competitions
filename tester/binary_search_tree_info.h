#pragma once

#include "common/binary_search_tree/info/rbt_color.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/sum.h"

inline size_t GetInfoValueI(const BSTInfoSize& info) { return info.size; }

inline size_t GetInfoValueI(const BSTInfoRBTColor<BSTInfoSize>& info) {
  return info.size;
}

template <class T1, class T2>
inline size_t GetInfoValueI(const BSTInfoSum<T1, T2>& info) {
  return info.sum;
}

template <class T1, class T2>
inline size_t GetInfoValueI(const BSTInfoRBTColor<BSTInfoSum<T1, T2>>& info) {
  return info.sum;
}

template <class TNode>
inline size_t GetInfoValue(const TNode* root) {
  return root ? GetInfoValueI(root->info) : 0;
}
