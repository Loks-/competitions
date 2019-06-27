#pragma once

#include "base/manipulator.h"
#include "base/rotation_type.h"
#include <vector>

namespace base {
class Map;
class Point;

class Manipulators {
 protected:
  std::vector<Manipulator> v;

 public:
  Manipulators();

  void Add(const Point& p);
  void Rotate(RotationType type);
  void Wrap(const Point& pworker, Map& map) const;
};
}  // namespace base
