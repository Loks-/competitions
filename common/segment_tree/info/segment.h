#pragma once

#include "common/segment_tree/info/none.h"

namespace st {
namespace info {
template <class TTCoordinate = unsigned>
class Segment : public None {
 public:
  using TCoordinate = TTCoordinate;
  using TBase = None;
  using TSelf = Segment<TCoordinate>;

  static const bool is_none = false;
  static const bool has_coordinate = true;

  TCoordinate left, right;

  void SetCoordinate(const TCoordinate& x) { left = right = x; }

  void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    left = l.left;
    right = r.right;
  }
};
}  // namespace info
}  // namespace st
