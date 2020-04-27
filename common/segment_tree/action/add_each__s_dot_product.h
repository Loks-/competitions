#pragma once

#include "common/segment_tree/action/none.h"

namespace st {
namespace action {
// Support info::SumFirst<T> and sinfo::SumSecond<Vector<T>>
template <class TData>
class AddEachSDotProduct : public None {
 public:
  using TBase = None;
  using TSelf = AddEachSDotProduct<TData>;
  static const bool is_none = false;

  TData add_value;

  bool IsEmpty() const { return add_value.IsZero(); }

  void Clear() {
    TBase::Clear();
    add_value.Clear();
  };

  template <class TNode>
  void Add(TNode* node, const TData& value) {
    node->info.sum += node->sinfo.sum.DotProduct(value);
    add_value += value;
  }

  template <class TNode>
  void Apply(TNode* node) {
    if (IsEmpty()) return;
    if (node->IsLeaf()) {
      node->GetData().first += node->sinfo.sum.DotProduct(add_value);
    } else {
      node->l->AddAction(add_value);
      node->r->AddAction(add_value);
    }
    Clear();
  }
};
}  // namespace action
}  // namespace st
