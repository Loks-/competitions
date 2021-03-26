#pragma once

#include "common/segment_tree/info/none.h"

namespace st {
namespace info {
template <class TSumValue, class TInfo = None>
class SumFirst : public TInfo {
 public:
  using TValue = TSumValue;
  using TBase = TInfo;
  using TSelf = SumFirst<TValue, TBase>;

  static const bool is_none = false;
  static const bool use_data = true;

  TValue sum;

  template <class TData>
  void UpdateData(const TData& data) {
    TBase::UpdateData(data);
    sum = data.first;
  }

  void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    sum = l.sum + r.sum;
  }
};
}  // namespace info
}  // namespace st
