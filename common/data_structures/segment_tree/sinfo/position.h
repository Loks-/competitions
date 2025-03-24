#pragma once

#include "common/data_structures/segment_tree/sinfo/none.h"

namespace ds {
namespace st {
namespace sinfo {
template <class TTCoordinate = unsigned>
class Position : public None {
 public:
  using TCoordinate = TTCoordinate;
  using TBase = None;
  using TSelf = Position<TCoordinate>;

 public:
  static constexpr bool empty = false;
  static constexpr bool has_coordinate = true;

 public:
  TCoordinate left, right;

 public:
  constexpr void SetCoordinate(const TCoordinate& l, const TCoordinate& r) {
    left = l;
    right = r;
  }

  constexpr TCoordinate Size() const { return right - left; }

  constexpr void UpdateLR(const TSelf& l, const TSelf& r) {
    TBase::UpdateLR(l, r);
    left = l.left;
    right = r.right;
  }
};
}  // namespace sinfo
}  // namespace st
}  // namespace ds
