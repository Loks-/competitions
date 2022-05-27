#pragma once

#include "common/data_structures/segment_tree/action/add_each.h"

namespace ds {
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
    node->info.max += value;
    TBase::Add(node, value);
  }
};
}  // namespace action
}  // namespace st
}  // namespace ds
