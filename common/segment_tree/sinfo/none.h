#pragma once

namespace st {
namespace sinfo {
class None {
 public:
  using TCoordinate = unsigned;
  using TSelf = None;

  static const bool is_none = true;
  static const bool use_data = false;
  static const bool has_coordinate = false;

  void SetCoordinate(const TCoordinate&) {}

  template <class TData>
  void UpdateData(const TData&) {}

  void UpdateLR(const TSelf&, const TSelf&) {}
};
}  // namespace sinfo
}  // namespace st
