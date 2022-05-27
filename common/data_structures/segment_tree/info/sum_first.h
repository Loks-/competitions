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

  static const bool is_none = false;
  static const bool use_data = true;

  TValue sum;

  template <class TData, class TSInfo>
  void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    sum = data.first;
  }

  template <class TSInfo>
  void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    sum = l.sum + r.sum;
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
