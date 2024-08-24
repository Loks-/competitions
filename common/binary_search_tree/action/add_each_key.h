#pragma once

#include "common/binary_search_tree/action/none.h"

namespace bst {
namespace action {
template <class TData>
class AddEachKey : public None {
 public:
  using TBase = None;
  using TSelf = AddEachKey<TData>;

  static constexpr bool is_none = false;
  static constexpr bool modify_keys = true;

 public:
  TData add_value_to_keys;

 public:
  constexpr AddEachKey() : add_value_to_keys() {}

  constexpr bool IsEmpty() const { return add_value_to_keys == TData(); }

  constexpr void Clear() { add_value_to_keys = TData(); };

  template <class TNode>
  constexpr void Add(TNode*, const TData& value) {
    add_value_to_keys += value;
  }

  template <class TNode>
  constexpr void Apply(TNode* node) {
    if (IsEmpty()) return;
    node->key += add_value_to_keys;
    if (node->l) node->l->AddAction(add_value_to_keys);
    if (node->r) node->r->AddAction(add_value_to_keys);
    add_value_to_keys = TData();
  }
};
}  // namespace action
}  // namespace bst
