#pragma once

namespace ds {
namespace st {
namespace sinfo {
class None {
 public:
  using TCoordinate = unsigned;
  using TSelf = None;

 public:
  static constexpr bool empty = true;
  static constexpr bool use_data = false;
  static constexpr bool has_coordinate = false;

 public:
  constexpr void SetCoordinate(const TCoordinate&, const TCoordinate&) {}

  // External functions should not assume that Update is constexpr.
  template <class TData>
  constexpr void UpdateData(const TData&) {}

  constexpr void UpdateLR(const TSelf&, const TSelf&) {}
};
}  // namespace sinfo
}  // namespace st
}  // namespace ds
