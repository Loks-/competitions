#pragma once

#include "common/data_structures/segment_tree/info/none.h"

namespace ds {
namespace st {
namespace info {
template <class TSumValue, class TInfo = None>
class SumFirst : public TInfo {
 public:
  using TValue = TSumValue;
  using TBase = TInfo;
  using TSelf = SumFirst<TValue, TBase>;

 public:
  static constexpr bool empty = false;
  static constexpr bool use_data = true;

 public:
  TValue sum_first;

 public:
  template <class TData, class TSInfo>
  constexpr void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    sum_first = data.first;
  }

  template <class TSInfo>
  constexpr void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    sum_first = l.sum_first + r.sum_first;
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
