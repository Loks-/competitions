#pragma once

#include "common/data_structures/segment_tree/info/none.h"

#include <algorithm>

namespace ds {
namespace st {
namespace info {
// Assume all data in leafs are non negative.
template <class TTValue, class TTSumValue, class TInfo = None>
class LSum0 : public None {
 public:
  using TValue = TTValue;
  using TSumValue = TTSumValue;
  using TBase = TInfo;
  using TSelf = LSum0<TValue, TSumValue, TBase>;

 public:
  static constexpr bool empty = false;
  static constexpr bool use_sinfo_in_update_data = true;
  static constexpr bool use_sinfo_in_update_lr = true;
  static constexpr bool use_update_node = true;

 public:
  TValue subtree_value;
  TSumValue lsum0;

 public:
  LSum0() { subtree_value = 0; }

  template <class TNode>
  void UpdateLSum0(const TNode* node) {
    lsum0 = subtree_value    ? 0
            : node->IsLeaf() ? node->sinfo.Size()
                             : node->l->info.lsum0 + node->r->info.lsum0;
  }

  template <class TNode>
  void UpdateNode(TNode* node) {
    TBase::UpdateNode(node);
    if (!node->IsLeaf()) {
      auto l = node->l, r = node->r;
      auto v = std::min(l->info.subtree_value, r->info.subtree_value);
      if (v) {
        l->info.subtree_value -= v;
        r->info.subtree_value -= v;
        subtree_value += v;
        l->info.UpdateLSum0(l);
        r->info.UpdateLSum0(r);
      }
    }
    UpdateLSum0(node);
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
