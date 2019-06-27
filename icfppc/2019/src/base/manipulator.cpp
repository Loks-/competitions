#include "base/manipulator.h"

#include "base/map.h"
#include "base/point.h"
#include "base/rotation.h"
#include "base/rotation_type.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/segment_intersect.h"
#include <algorithm>
#include <cmath>

namespace base {
Manipulator::Manipulator(Point _p) {
  p = _p;
  int x0 = std::min(p.x, 0);
  int x1 = std::max(p.x, 0);
  int y0 = std::min(p.y, 0);
  int y1 = std::max(p.y, 0);
  I2OpenSegment s0({0, 0}, {2 * p.x, 2 * p.y});
  for (int x = x0; x <= x1; ++x) {
    for (int y = y0; y <= y1; ++y) {
      if ((x == 0) && (y == 0)) continue;
      I2OpenSegment s1({2 * x - 1, 2 * y - 1}, {2 * x + 1, 2 * y + 1}),
          s2({2 * x - 1, 2 * y + 1}, {2 * x + 1, 2 * y - 1});
      if (Intersect(s0, s1) || Intersect(s0, s2)) {
        check_list.push_back(Point{x, y});
      }
    }
  }
  check_list.push_back(p);
}

void Manipulator::Rotate(RotationType type) {
  Rotation r(type);
  p = r(p);
  for (Point& pc : check_list) pc = r(pc);
}

void Manipulator::Wrap(const Point& pworker, Map& map) const {
  bool visible = true;
  for (const Point& pc : check_list) {
    Point pf{pworker.x + pc.x, pworker.y + pc.y};
    if (map.Obstacle(pf)) {
      visible = false;
      break;
    }
  }
  if (visible) map.Wrap(p);
}

bool Manipulator::operator==(const Point& _p) const { return p == _p; }

bool Manipulator::Neighbor(const Point& _p) const {
  return std::abs(p.x - _p.x) + std::abs(p.y - _p.y) == 1;
}
}  // namespace base
