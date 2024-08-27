#pragma once

#include "common/data_structures/segment_tree/info/lsum0.h"
#include "common/data_structures/segment_tree/info/none.h"

#include <algorithm>

namespace ds {
namespace st {
namespace info {
// Assume all data in leafs are non negative.
template <class TTValue, class TTSumValue, class TInfo = None>
class LSum1 : public LSum0<TTValue, TTSumValue, TInfo> {
 public:
  using TValue = TTValue;
  using TSumValue = TTSumValue;
  using TBase = LSum0<TTValue, TTSumValue, TInfo>;
  using TSelf = LSum0<TValue, TSumValue, TInfo>;

 public:
  TSumValue lsum1;

 public:
  template <class TNode>
  void UpdateLSum1(const TNode* node) {
    if (TBase::subtree_value == 0) {
      lsum1 = node->IsLeaf() ? 0 : node->l->info.lsum1 + node->r->info.lsum1;
    } else if (TBase::subtree_value == 1) {
      lsum1 = node->IsLeaf() ? node->sinfo.Size()
                             : node->l->info.lsum0 + node->r->info.lsum0;
    } else {
      lsum1 = TSumValue();
    }
  }

  template <class TNode>
  void UpdateNode(TNode* node) {
    TBase::TBase::UpdateNode(node);
    if (!node->IsLeaf()) {
      auto l = node->l, r = node->r;
      auto v = std::min(l->info.subtree_value, r->info.subtree_value);
      if (v) {
        l->info.subtree_value -= v;
        r->info.subtree_value -= v;
        TBase::subtree_value += v;
        l->info.UpdateLSum0(l);
        r->info.UpdateLSum0(r);
        l->info.UpdateLSum1(l);
        r->info.UpdateLSum1(r);
      }
    }
    TBase::UpdateLSum0(node);
    UpdateLSum1(node);
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
