#pragma once

#include "common/segment_tree/action/none.h"

namespace st {
namespace action {
template <class TData>
class AddEach : public None {
 public:
  using TBase = None;
  using TSelf = AddEach<TData>;
  static const bool is_none = false;

  TData add_value;

  AddEach() : add_value() {}

  bool IsEmpty() const { return add_value == TData(); }
  void Clear() { add_value = TData(); };

  template <class TNode>
  void Add(TNode* node, const TData& value) {
    add_value += value;
  }

  template <class TNode>
  void Apply(TNode* node) {
    if (IsEmpty()) return;
    if (node->IsLeaf()) {
      node->GetData() += add_value;
    } else {
      node->l->AddAction(add_value);
      node->r->AddAction(add_value);
    }
    add_value = TData();
  }
};
}  // namespace action
}  // namespace st
