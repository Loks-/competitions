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

  TValue max;

  template <class TData>
  void UpdateData(const TData& data) {
    TBase::UpdateData(data);
    max = data;
  }

  void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    max = std::max(l.max, r.max);
  }
};
}  // namespace info
}  // namespace st
