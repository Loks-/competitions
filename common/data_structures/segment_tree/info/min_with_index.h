#pragma once

namespace ds {
namespace st {
namespace info {
template <class TMinValue, class TInfo>
class MinWithIndex : public TInfo {
 public:
  using TValue = TMinValue;
  using TBase = TInfo;
  using TSelf = MinWithIndex<TValue, TBase>;

  static const bool is_none = false;
  static const bool use_data = true;
  static const bool use_sinfo_in_update_data = true;

  TValue min;
  unsigned min_coordinate;

  template <class TData, class TSInfo>
  void UpdateData(const TData& data, const TSInfo& sinfo) {
    static_assert(TSInfo::has_coordinate);
    TBase::UpdateData(data, sinfo);
    min = data;
    min_coordinate = sinfo.left;
  }

  template <class TSInfo>
  void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    if (r.min < l.min) {
      min = r.min;
      min_coordinate = r.min_coordinate;
    } else {
      min = l.min;
      min_coordinate = r.min_coordinate;
    }
  }
};
}  // namespace info
}  // namespace st
}  // namespace ds
