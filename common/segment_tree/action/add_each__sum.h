#pragma once

#include "common/segment_tree/action/add_each.h"

namespace st {
namespace action {
// Support info::Sum
template <class TData>
class AddEachSum : public AddEach<TData> {
 public:
  using TBase = AddEach<TData>;
  using TSelf = AddEachSum<TData>;

  template <class TNode>
  void Add(TNode* node, const TData& value) {
    node->info.sum += node->info.Size() * value;
    TBase::Add(node, value);
  }
};
}  // namespace action
}  // namespace st
