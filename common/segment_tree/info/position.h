#pragma once

#include "common/segment_tree/info/none.h"

namespace st {
namespace info {
template <class TTCoordinate = unsigned>
class Position : public None {
 public:
  using TCoordinate = TTCoordinate;
  using TBase = None;
  using TSelf = Position<TCoordinate>;

  static const bool is_none = false;
  static const bool has_coordinate = true;

  TCoordinate left, right;

  void SetCoordinate(const TCoordinate& x) { left = right = x; }
  TCoordinate Size() const { return right - left + 1; }

  void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    left = l.left;
    right = r.right;
  }
};
}  // namespace info
}  // namespace st
