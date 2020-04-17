#pragma once

#include "common/segment_tree/action/add_each.h"

namespace st {
namespace action {
// Support info::Max
template <class TData>
class AddEachMax : public AddEach<TData> {
 public:
  using TBase = AddEach<TData>;
  using TSelf = AddEachMax<TData>;

  template <class TNode>
  void Add(TNode* node, const TData& value) {
    node->info.segment_max += value;
    TBase::Add(node, value);
  }
};
}  // namespace action
}  // namespace st
