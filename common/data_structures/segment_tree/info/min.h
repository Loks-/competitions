#pragma once

#include "common/data_structures/segment_tree/info/none.h"

#include <algorithm>

namespace ds {
namespace st {
namespace info {
template <class TMinValue, class TInfo = None>
class Min : public TInfo {
 public:
  using TValue = TMinValue;
  using TBase = TInfo;
  using TSelf = Min<TValue, TBase>;

 public:
  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  TValue min;

 public:
  template <class TData, class TSInfo>
  constexpr void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    min = data;
  }

  template <class TSInfo>
  constexpr void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    min = std::min(l.min, r.min);
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
