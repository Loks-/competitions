#pragma once

#include "common/segment_tree/info/proxy.h"
#include <algorithm>

namespace st {
namespace info {
template <class TMaxValue, class TInfo>
class TMax : public TInfo {
 public:
  using TValue = TMaxValue;
  using TBase = TInfo;
  using TSelf = TMax<TValue, TBase>;

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
using Max = Proxy<TMax<TMaxValue, TInfo>>;
}  // namespace info
}  // namespace st
