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
  static const bool support_remove = false;

  TGCDType gcd;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    gcd = ::GCD<TGCDType>(node->data,
                          ::GCD<TGCDType>((node->l ? node->l->info.gcd : 0),
                                          (node->r ? node->r->info.gcd : 0)));
  }

  template <class TNode>
  void Insert(const TNode* node) {
    TBase::Insert(node);
    gcd = ::GCD<TGCDType>(gcd, node->info.gcd);
  }
};
}  // namespace info
}  // namespace bst
