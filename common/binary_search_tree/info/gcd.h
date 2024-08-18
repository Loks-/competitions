#pragma once

#include "common/numeric/utils/gcd.h"

namespace bst {
namespace info {
template <class TGCDType, class TInfo>
class GCD : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = GCD<TGCDType, TInfo>;

  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  TGCDType gcd;

 public:
  template <class TNode>
  constexpr void Update(TNode* node) {
    TBase::Update(node);
    gcd = TGCDType(node->data);
    if (node->l) gcd = ::GCD(gcd, node->l->info.gcd);
    if (node->r) gcd = ::GCD(gcd, node->r->info.gcd);
  }
};
}  // namespace info
}  // namespace bst
