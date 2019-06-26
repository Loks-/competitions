#include "base/manipulator.h"

#include "base/map.h"
#include "base/point.h"
#include "base/rotation.h"
#include "base/rotation_type.h"
#include <algorithm>

namespace base {
Manipulator::Manipulator(Point _p) {
  p = _p;
  int x0 = std::min(p.x, 0);
  int x1 = std::max(p.x, 0);
  int y0 = std::min(p.y, 0);
  int y1 = std::max(p.y, 0);
  for (int x = x0; x <= x1; ++x) {
    for (int y = y0; y <= y1; ++y) {
      if ((x == 0) && (y == 0)) continue;
      // ...
      // Real check using common library
    }
  }
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
}  // namespace base
