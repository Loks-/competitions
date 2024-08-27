#pragma once

#include "common/data_structures/segment_tree/action/none.h"

namespace ds {
namespace st {
namespace action {
template <class TData>
class AddEach : public None {
 public:
  using TBase = None;
  using TSelf = AddEach<TData>;

  static constexpr bool is_none = false;
  static constexpr bool modify_data = true;

 public:
  TData add_value;

 public:
  constexpr AddEach() : add_value() {}

  constexpr bool IsEmpty() const { return add_value == TData(); }

  constexpr void Clear() {
    TBase::Clear();
    add_value = TData();
  };

  template <class TNode>
  constexpr void Add(TNode*, const TData& value) {
    add_value += value;
  }

  template <class TNode>
  constexpr void Apply(TNode* node) {
    if (IsEmpty()) return;
    if (node->IsLeaf()) {
      node->GetData() += add_value;
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
