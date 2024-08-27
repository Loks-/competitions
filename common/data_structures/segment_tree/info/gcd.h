#pragma once

#include "common/data_structures/segment_tree/info/none.h"
#include "common/numeric/utils/gcd.h"

namespace ds {
namespace st {
namespace info {
template <class TGCDValue, class TInfo = None>
class GCD : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = GCD<TGCDValue, TBase>;

 public:
  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  TGCDValue gcd;

 public:
  template <class TData, class TSInfo>
  constexpr void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    gcd = TGCDValue(data);
  }

  template <class TSInfo>
  constexpr void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    gcd = ::GCD(l.gcd, r.gcd);
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
