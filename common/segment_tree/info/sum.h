#pragma once

#include "common/segment_tree/info/proxy.h"

namespace st {
namespace info {
template <class TSumValue, class TInfo>
class TSum : public TInfo {
 public:
  using TValue = TSumValue;
  using TBase = TInfo;
  using TSelf = TSum<TValue, TBase>;

  static const bool is_none = false;
  static const bool use_data = true;

  TValue segment_sum;

  template <class TData>
  void UpdateData(const TData& data) {
    TBase::UpdateData(data);
    segment_sum = data;
  }

  void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    segment_sum = l.segment_sum + r.segment_sum;
  }
};

template <class TSumValue, class TInfo>
using Sum = Proxy<TSum<TSumValue, TInfo>>;
}  // namespace info
}  // namespace st
