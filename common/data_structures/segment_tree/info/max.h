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

  static const bool is_none = false;
  static const bool use_data = true;

  TValue max;

  template <class TData, class TSInfo>
  void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    max = data;
  }

  template <class TSInfo>
  void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    max = std::max(l.max, r.max);
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
