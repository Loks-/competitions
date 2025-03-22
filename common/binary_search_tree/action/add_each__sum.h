#pragma once

#include "common/binary_search_tree/action/add_each.h"

namespace bst {
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
    node->subtree_data.sum_value += value * node->subtree_data.size;
    TBase::Add(node, value);
  }
};
}  // namespace action
}  // namespace bst
