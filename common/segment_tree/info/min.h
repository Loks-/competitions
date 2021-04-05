#pragma once

#include "common/segment_tree/info/none.h"

#include <algorithm>

namespace st {
namespace info {
template <class TMinValue, class TInfo = None>
class Min : public TInfo {
 public:
  using TValue = TMinValue;
  using TBase = TInfo;
  using TSelf = Min<TValue, TBase>;

  static const bool is_none = false;
  static const bool use_data = true;

  TValue min;

  template <class TData, class TSInfo>
  void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    min = data;
  }

  template <class TSInfo>
  void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    min = std::min(l.min, r.min);
  }
};
}  // namespace info
}  // namespace st
