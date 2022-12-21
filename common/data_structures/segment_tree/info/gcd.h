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

  static const bool is_none = false;
  static const bool use_data = true;

  TGCDValue gcd;

  template <class TData, class TSInfo>
  void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    gcd = TGCDValue(data);
  }

  template <class TSInfo>
  void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    gcd = ::GCD(l.gcd, r.gcd);
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
