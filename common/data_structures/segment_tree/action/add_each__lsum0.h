#pragma once

#include "common/base.h"
#include "common/data_structures/segment_tree/action/none.h"

namespace ds {
namespace st {
namespace action {
template <class TValue>
class AddEachLSum0 : public None {
 public:
  using TBase = None;
  using TSelf = AddEachLSum0<TValue>;

  template <class TNode>
  void Add(TNode* node, const TValue& value) {
    TBase::Add(node, value);
    node->info.subtree_value += value;
    node->UpdateInfo();
  }
};
}  // namespace action
}  // namespace st
}  // namespace ds
