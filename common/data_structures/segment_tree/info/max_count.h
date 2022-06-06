#pragma once

#include "common/data_structures/segment_tree/info/max.h"
#include "common/data_structures/segment_tree/info/none.h"

namespace ds {
namespace st {
namespace info {
template <class TMaxValue, class TInfo = None>
class MaxCount : public Max<TMaxValue, TInfo> {
 public:
  using TBase = Max<TMaxValue, TInfo>;
  using TSelf = MaxCount<TMaxValue, TInfo>;

  unsigned max_count;

  template <class TData, class TSInfo>
  void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    max_count = 1;
  }

  template <class TSInfo>
  void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    max_count = ((l.max == TBase::max) ? l.max_count : 0u) +
                ((r.max == TBase::max) ? r.max_count : 0u);
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
