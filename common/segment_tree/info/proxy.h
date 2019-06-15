#pragma once

#include "common/base.h"

namespace st {
namespace info {
template <class TTBase>
class Proxy : public TTBase {
 public:
  using TBase = TTBase;
  using TSelf = Proxy<TBase>;

  template <class TNode>
  void Update(TNode* node) {
    assert(node);
    node->ApplyAction();
    if (node->IsLeaf())
      TBase::UpdateData(node->GetData());
    else
      TBase::UpdateLR(node->l->info, node->r->info);
  }

  static TSelf MergeLR(const TSelf& l, const TSelf& r) {
    TSelf t;
    t.UpdateLR(l, r);
    return t;
  }
};
}  // namespace info
}  // namespace st
