#pragma once

#include "common/binary_search_tree/action/none.h"

namespace bst {
namespace action {
template <class TData>
class AddEachKey : public None {
 public:
  using TBase = None;
  using TSelf = AddEachKey<TData>;
  static const bool modify_data = true;

  TData add_value;

  AddEachKey() : add_value() {}
  bool IsEmpty() const { return add_value == TData(); }
  void Clear() { add_value = TData(); };

  template <class TNode>
  void Add(TNode*, const TData& value) {
    add_value += value;
  }

  template <class TNode>
  void Apply(TNode* node) {
    if (IsEmpty()) return;
    node->key += add_value;
    if (node->l) node->l->AddAction(add_value);
    if (node->r) node->r->AddAction(add_value);
    add_value = TData();
  }
};
}  // namespace action
}  // namespace bst
