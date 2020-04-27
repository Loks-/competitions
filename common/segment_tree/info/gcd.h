#pragma once

#include "common/numeric/utils/gcd.h"
#include "common/segment_tree/info/none.h"

namespace st {
namespace info {
template <class TGCDValue, class TInfo = None>
class GCD : public TInfo {
 public:
  using TValue = TGCDValue;
  using TBase = TInfo;
  using TSelf = GCD<TValue, TBase>;

  static const bool is_none = false;
  static const bool use_data = true;

  TValue gcd;

  template <class TData>
  void UpdateData(const TData& data) {
    TBase::UpdateData(data);
    gcd = data;
  }

  void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    gcd = ::GCD<TValue>(l.gcd, r.gcd);
  }
};
}  // namespace info
}  // namespace st
