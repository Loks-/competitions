#pragma once

#include "common/data_structures/segment_tree/info/none.h"
#include "common/numeric/utils/gcd.h"

namespace ds {
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

  template <class TData, class TSInfo>
  void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    gcd = data;
  }

  template <class TSInfo>
  void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    gcd = ::GCD<TValue>(l.gcd, r.gcd);
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
