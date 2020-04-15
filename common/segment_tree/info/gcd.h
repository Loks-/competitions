#pragma once

#include "common/numeric/utils/gcd.h"

namespace st {
namespace info {
template <class TGCDValue, class TInfo>
class GCD : public TInfo {
 public:
  using TValue = TGCDValue;
  using TBase = TInfo;
  using TSelf = GCD<TValue, TBase>;

  static const bool is_none = false;
  static const bool use_data = true;

  TValue segment_gcd;

  template <class TData>
  void UpdateData(const TData& data) {
    TBase::UpdateData(data);
    segment_gcd = data;
  }

  void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    segment_gcd = ::GCD<TValue>(l.segment_gcd, r.segment_gcd);
  }
};
}  // namespace info
}  // namespace st
