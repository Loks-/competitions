#pragma once

#include "common/segment_tree/info/proxy.h"

namespace st {
namespace info {
class TNone {
 public:
  using TCoordinate = unsigned;
  using TSelf = TNone;

  static const bool is_none = true;
  static const bool use_data = false;
  static const bool has_coordinate = false;

  void SetCoordinate(const TCoordinate& x) {}

  template <class TData>
  void UpdateData(const TData& data) {}

  void UpdateLR(const TSelf& l, const TSelf& r) {}
};

using None = Proxy<TNone>;
}  // namespace info
}  // namespace st
