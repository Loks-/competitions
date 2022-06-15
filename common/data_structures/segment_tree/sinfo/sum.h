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

  static const bool is_none = false;
  static const bool use_data = true;

  TValue sum;

  template <class TData>
  void UpdateData(const TData& data) {
    TBase::UpdateData(data);
    sum = data;
  }

  void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    sum = l.sum + r.sum;
  }
};
}  // namespace sinfo
}  // namespace st
}  // namespace ds
