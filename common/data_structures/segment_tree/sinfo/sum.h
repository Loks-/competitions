#pragma once

namespace ds {
namespace st {
namespace sinfo {
template <class TSumValue, class TInfo>
class Sum : public TInfo {
 public:
  using TValue = TSumValue;
  using TBase = TInfo;
  using TSelf = Sum<TValue, TBase>;

 public:
  static constexpr bool empty = false;
  static constexpr bool use_data = true;

 public:
  TValue sum;

 public:
  template <class TData>
  constexpr void UpdateData(const TData& data) {
    TBase::UpdateData(data);
    sum = data;
  }

  constexpr void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    sum = l.sum + r.sum;
  }
};
}  // namespace sinfo
}  // namespace st
}  // namespace ds
