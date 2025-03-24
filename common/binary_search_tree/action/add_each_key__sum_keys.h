#pragma once

#include "common/binary_search_tree/action/add_each_key.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace action {
// Support info::SumKeys
template <class TAggregatorSum, class TData>
class AddEachKeySumKeys : public AddEachKey<TData> {
 public:
  using TBase = AddEachKey<TData>;
  using TSelf = AddEachKeySumKeys<TAggregatorSum, TData>;

 public:
  template <class TNode>
  constexpr void Add(TNode* node, const TData& value) {
    TAggregatorSum::get_ref(node) += value * bst::subtree_data::size(node);
    TBase::Add(node, value);
  }
};
}  // namespace action
}  // namespace bst
