#pragma once

#include "common/data_structures/segment_tree/action/add_each.h"

namespace ds {
namespace st {
namespace action {
// Support info::Sum
template <class TData>
class AddEachSum : public AddEach<TData> {
 public:
  using TBase = AddEach<TData>;
  using TSelf = AddEachSum<TData>;

 public:
  template <class TNode>
  constexpr void Add(TNode* node, const TData& value) {
    node->info.sum += value * node->sinfo.Size();
    TBase::Add(node, value);
  }
};
}  // namespace action
}  // namespace st
}  // namespace ds
