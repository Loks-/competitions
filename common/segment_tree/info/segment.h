#pragma once

#include "common/segment_tree/info/none.h"
#include "common/segment_tree/info/proxy.h"

template <class TTCoordinate = unsigned>
class TSTInfoSegment : public TSTInfoNone {
 public:
  using TCoordinate = TTCoordinate;
  using TBase = TSTInfoNone;
  using TSelf = TSTInfoSegment<TCoordinate>;

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

template <class TCoordinate = unsigned>
using STInfoSegment = STInfoProxy<TSTInfoSegment<TCoordinate>>;
