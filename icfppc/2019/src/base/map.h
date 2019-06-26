#pragma once

#include "base/point.h"
#include <cstdint>
#include <vector>

namespace base {
class Map {
 protected:
  static const uint8_t OBSTACLE = 1;
  static const uint8_t WRAPPED = 2;

 protected:
  int xsize;
  int ysize;
  std::vector<uint8_t> map;

 public:
  unsigned Index(const Point& p) const;
  Point GetPoint(unsigned index) const;

  bool Inside(const Point& p) const;
  bool Obstacle(const Point& p) const;
  void Wrap(const Point& p);
};
}  // namespace base
