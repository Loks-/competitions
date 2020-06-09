#pragma once

#include "common/graph/tree/lcte/action/add_each.h"

namespace graph {
namespace lcte {
namespace action {
// Support info::Max
template <class TData>
class AddEachMax : public AddEach<TData> {
 public:
  using TBase = AddEach<TData>;
  using TSelf = AddEachMax<TData>;

  template <class TNode>
  void PAdd(TNode* node, const TData& value) {
    auto& i = node->info;
    if (i.cmax_set) i.cmax += value;
    i.pmax += value;
    if (i.vmax_set) i.vmax += value;
    i.tmax += value;
    TBase::PAdd(node, value);
  }

  template <class TNode>
  void VAdd(TNode* node, const TData& value) {
    node->info.vmax += value;
    TBase::VAdd(node, value);
  }
};
}  // namespace action
}  // namespace lcte
}  // namespace graph
