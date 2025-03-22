#pragma once

#include "common/binary_search_tree/action/add_each_key.h"

namespace bst {
namespace action {
// Support info::SumKeys
template <class TData>
class AddEachKeySumKeys : public AddEachKey<TData> {
 public:
  using TBase = AddEachKey<TData>;
  using TSelf = AddEachKeySumKeys<TData>;

 public:
  template <class TNode>
  constexpr void Add(TNode* node, const TData& value) {
    node->subtree_data.sum_keys += value * node->subtree_data.size;
    TBase::Add(node, value);
  }
};
}  // namespace action
}  // namespace bst
