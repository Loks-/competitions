#pragma once

#include "common/numeric/utils/gcd.h"

namespace bst {
namespace info {
template <class TGCDType, class TInfo>
class GCD : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = GCD<TGCDType, TInfo>;
  static const bool is_none = false;
  static const bool use_data = true;

  TGCDType gcd;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    gcd = TGCDType(node->data);
    if (node->l) gcd = ::GCD(gcd, node->l->info.gcd);
    if (node->r) gcd = ::GCD(gcd, node->r->info.gcd);
  }
};
}  // namespace info
}  // namespace bst
