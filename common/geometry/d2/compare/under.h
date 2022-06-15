#pragma once

#include "common/geometry/d2/point.h"

template <class TTValue>
inline bool Under(const geometry::d2::Point<TTValue>& l,
                  const geometry::d2::Point<TTValue>& r) {
  return (l.x <= r.x) && (l.y <= r.y);
}

template <class TTValue>
inline bool StrictUnder(const geometry::d2::Point<TTValue>& l,
                        const geometry::d2::Point<TTValue>& r) {
  return (l.x < r.x) && (l.y < r.y);
}
