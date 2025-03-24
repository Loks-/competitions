#pragma once

#include "common/binary_search_tree/action/add_each.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace action {
// Support info::Sum
template <class TAggregatorSum, class TData>
class AddEachSum : public AddEach<TData> {
 public:
  using TBase = AddEach<TData>;
  using TSelf = AddEachSum<TAggregatorSum, TData>;

 public:
  template <class TNode>
  constexpr void Add(TNode* node, const TData& value) {
    TAggregatorSum::get_ref(node) += value * bst::subtree_data::size(node);
    TBase::Add(node, value);
  }
};
}  // namespace action
}  // namespace bst
