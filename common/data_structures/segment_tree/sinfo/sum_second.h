#pragma once

namespace ds {
namespace st {
namespace sinfo {
template <class TSumValue, class TInfo>
class SumSecond : public TInfo {
 public:
  using TValue = TSumValue;
  using TBase = TInfo;
  using TSelf = SumSecond<TValue, TBase>;

 public:
  static constexpr bool empty = false;
  static constexpr bool use_data = true;

 public:
  TValue sum_second;

 public:
  template <class TData>
  constexpr void UpdateData(const TData& data) {
    TBase::UpdateData(data);
    sum_second = data.second;
  }

  constexpr void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    sum_second = l.sum_second + r.sum_second;
  }
};
}  // namespace sinfo
}  // namespace st
}  // namespace ds
