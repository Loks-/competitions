#pragma once

#include "common/geometry/d2/vector.h"

#include <vector>

inline const std::vector<I2Vector>& I2NeighborsD4() {
  static std::vector<I2Vector> v{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  return v;
}
