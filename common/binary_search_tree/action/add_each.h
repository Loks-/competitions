#pragma once

#include "common/binary_search_tree/action/none.h"

namespace bst {
namespace action {
template <class TData>
class AddEach : public None {
 public:
  using TBase = None;
  using TSelf = AddEach<TData>;
  static const bool is_none = false;

  TData add_value;

  AddEach() : add_value(0) {}
  bool IsEmpty() const { return add_value == 0; }
  void Clear() { add_value = 0; };

  template <class TNode>
  void Add(TNode* node, const TData& value) {
    node->info.sum += node->info.size * value;
    add_value += value;
  }

  template <class TNode>
  void Apply(TNode* node) {
    if (IsEmpty()) return;
    node->data += add_value;
    if (node->l) node->l->AddAction(add_value);
    if (node->r) node->r->AddAction(add_value);
    add_value = 0;
  }
};
}  // namespace action
}  // namespace bst
