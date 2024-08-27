#pragma once

#include "common/data_structures/segment_tree/action/none.h"

namespace ds {
namespace st {
namespace action {
// Support info::SumFirst<T> and sinfo::SumSecond<Vector<T>>
template <class TData>
class AddEachSDotProduct : public None {
 public:
  using TBase = None;
  using TSelf = AddEachSDotProduct<TData>;

  static constexpr bool is_none = false;
  static constexpr bool modify_data = true;

 public:
  TData add_value;

 public:
  constexpr bool IsEmpty() const { return add_value.IsZero(); }

  constexpr void Clear() {
    TBase::Clear();
    add_value.Clear();
  };

  template <class TNode>
  constexpr void Add(TNode* node, const TData& value) {
    node->info.sum_first += node->sinfo.sum_second.DotProduct(value);
    add_value += value;
  }

  template <class TNode>
  constexpr void Apply(TNode* node) {
    if (IsEmpty()) return;
    if (node->IsLeaf()) {
      node->GetData().first += node->sinfo.sum_second.DotProduct(add_value);
    } else {
      node->l->AddAction(add_value);
      node->r->AddAction(add_value);
    }
    Clear();
  }
};
}  // namespace action
}  // namespace st
}  // namespace ds
