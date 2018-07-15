#pragma once

#include "common/binary_search_tree/info.h"
#include "common/binary_search_tree/red_black_tree.h"

namespace {

size_t GetInfoValue(const BSTInfoSize& info) { return info.size; }
size_t GetInfoValue(const RBTInfo<BSTInfoSize>& info) { return info.size; }

template<class TNode>
size_t GetInfoValue(const TNode* root) { return root ? GetInfoValue(root->info) : 0; }

} // namespace
