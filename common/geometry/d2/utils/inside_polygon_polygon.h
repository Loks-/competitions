#pragma once

#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/inside_segment_polygon.h"

namespace geometry {
namespace d2 {
template <class T>
constexpr bool Inside(const Polygon<T>& plgn_small,
                      const Polygon<T>& plgn_large) {
  for (unsigned i = 0; i < plgn_small.Size(); ++i) {
    if (!Inside(Segment<T, true>(plgn_small[i], plgn_small.MGet(i + 1)),
                plgn_large))
      return false;
  }
  return true;
}
}  // namespace d2
}  // namespace geometry
