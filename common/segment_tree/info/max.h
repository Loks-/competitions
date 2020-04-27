#pragma once

#include "common/segment_tree/info/none.h"

#include <algorithm>

namespace st {
namespace info {
template <class TMaxValue, class TInfo = None>
class Max : public TInfo {
 public:
  using TValue = TMaxValue;
  using TBase = TInfo;
  using TSelf = Max<TValue, TBase>;

  static const bool is_none = false;
  static const bool use_data = true;

  TValue segment_max;

  template <class TData>
  void UpdateData(const TData& data) {
    TBase::UpdateData(data);
    segment_max = data;
  }

  void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    segment_max = std::max(l.segment_max, r.segment_max);
  }
};
}  // namespace info
}  // namespace st
