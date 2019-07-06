#pragma once

#include "base/point.h"
#include "base/rotation_type.h"
#include <vector>

namespace base {
class Manipulator {
 protected:
  Point p;
  std::vector<Point> check_list;

 public:
  Manipulator(Point p);

  void Rotate(RotationType type);

  template <class TMap>
  void Wrap(const Point& pworker, TMap& map) const;

  bool operator==(const Point& _p) const;
  bool Neighbor(const Point& _p) const;
};
}  // namespace base