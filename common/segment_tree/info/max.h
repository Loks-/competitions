#pragma once

#include "proxy.h"
#include <algorithm>

template <class TMaxValue, class TInfo>
class TSTInfoMax : public TInfo {
 public:
  using TValue = TMaxValue;
  using TBase = TInfo;
  using TSelf = TSTInfoMax<TValue, TBase>;

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

template <class TMaxValue, class TInfo>
using STInfoMax = STInfoProxy<TSTInfoMax<TMaxValue, TInfo>>;
