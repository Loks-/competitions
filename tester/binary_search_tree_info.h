#pragma once

#include "common/binary_search_tree/info.h"
#include "common/binary_search_tree/red_black_tree.h"
#include "common/binary_search_tree/info/sum.h"

namespace {

size_t GetInfoValueI(const BSTInfoSize& info) { return info.size; }
size_t GetInfoValueI(const RBTInfo<BSTInfoSize>& info) { return info.size; }
template<class T1, class T2>
size_t GetInfoValueI(const BSTInfoSum<T1, T2>& info) { return info.sum; }
template<class T1, class T2>
size_t GetInfoValueI(const RBTInfo<BSTInfoSum<T1, T2>>& info) { return info.sum; }

template<class TNode>
size_t GetInfoValue(const TNode* root) { return root ? GetInfoValueI(root->info) : 0; }

} // namespace
