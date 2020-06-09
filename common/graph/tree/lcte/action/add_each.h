#pragma once

#include "common/graph/tree/lcte/action/none.h"

namespace graph {
namespace lcte {
namespace action {
template <class TData>
class AddEach : public None {
 public:
  using TBase = None;
  using TSelf = AddEach<TData>;

  TData add_value;

  AddEach() { ClearValue(); }
  bool IsEmptyValue() const { return add_value == TData(); }
  bool IsEmpty() const { return TBase::IsEmpty() && IsEmptyValue(); }
  void ClearValue() { add_value = TData(); }
  void Clear() {
    TBase::Clear();
    ClearValue();
  };

  template <class TNode>
  void PAdd(TNode*, const TData& value) {
    add_value += value;
  }

  template <class TNode>
  void VAdd(TNode*, const TData& value) {
    add_value += value;
  }

  template <class TNode>
  void PApply(TNode* node) {
    TBase::PApply(node);
    if (!IsEmptyValue()) {
      node->data += add_value;
      if (node->l) node->l->AddAction(add_value);
      if (node->r) node->r->AddAction(add_value);
      if (node->vc) node->vc->AddAction(add_value);
      ClearValue();
    }
  }

  template <class TNode>
  void VApply(TNode* node) {
    TBase::VApply(node);
    if (!IsEmptyValue()) {
      if (node->l) node->l->AddAction(add_value);
      if (node->r) node->r->AddAction(add_value);
      node->vc->AddAction(add_value);
      ClearValue();
    }
  }
};
}  // namespace action
}  // namespace lcte
}  // namespace graph
