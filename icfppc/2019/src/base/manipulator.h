#pragma once

#include "base/point.h"
#include "base/rotation_type.h"
#include <vector>

namespace base {
class Map;

class Manipulator {
 protected:
  Point p;
  std::vector<Point> check_list;

 public:
  Manipulator(Point p);

  void Rotate(RotationType type);
  void Wrap(const Point& pworker, Map& map) const;
};
}  // namespace base
