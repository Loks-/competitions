#pragma once

#include "common/binary_search_tree/action/none.h"

namespace bst {
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

  constexpr void Clear() { add_value = TData(); };

  template <class TNode>
  constexpr void Add(TNode*, const TData& value) {
    add_value += value;
  }

  template <class TNode>
  constexpr void Apply(TNode* node) {
    if (IsEmpty()) return;
    node->data += add_value;
    if (node->l) node->l->AddAction(add_value);
    if (node->r) node->r->AddAction(add_value);
    add_value = TData();
  }
};
}  // namespace action
}  // namespace bst
