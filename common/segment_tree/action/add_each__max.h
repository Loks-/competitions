#pragma once

#include "common/segment_tree/action/none.h"

namespace st {
namespace action {
// Support info::Max
template <class TData>
class AddEachMax : public None {
 public:
  using TBase = None;
  using TSelf = AddEachMax<TData>;
  static const bool is_none = false;

  TData add_value;

  AddEachMax() : add_value(0) {}

  bool IsEmpty() const { return add_value == 0; }
  void Clear() { add_value = 0; };

  template <class TNode>
  void Add(TNode* node, const TData& value) {
    node->info.segment_max += value;
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
    add_value = 0;
  }
};
}  // namespace action
}  // namespace st
