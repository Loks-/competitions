#pragma once

#include "common/files/json.h"
#include "common/geometry/d2/point.h"

#include <vector>

inline I2Point GetPoint(const files::JSON& json) {
  return I2Point(json.GetInteger(0), json.GetInteger(1));
}

inline files::JSON ToJSON(const I2Point& p) {
  return files::JSON(
      std::vector<files::JSON>{files::JSON(int(p.x)), files::JSON(int(p.y))});
}
