#pragma once

#include "common/data_structures/segment_tree/info/none.h"

#include <algorithm>

namespace ds {
namespace st {
namespace info {
template <class TMaxValue, class TInfo = None>
class Max : public TInfo {
 public:
  using TValue = TMaxValue;
  using TBase = TInfo;
  using TSelf = Max<TValue, TBase>;

 public:
  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  TValue max;

 public:
  template <class TData, class TSInfo>
  constexpr void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    max = data;
  }

  template <class TSInfo>
  constexpr void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    max = std::max(l.max, r.max);
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
