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
  static const bool modify_data = true;

  TData add_value;

  AddEach() : add_value() {}

  bool IsEmpty() const { return add_value == TData(); }

  void Clear() {
    TBase::Clear();
    add_value = TData();
  };

  template <class TNode>
  void Add(TNode*, const TData& value) {
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
    Clear();
  }
};
}  // namespace action
}  // namespace st
}  // namespace ds
