#pragma once

namespace base {
class Point {
 public:
  int x;
  int y;

  bool operator==(const Point& p) const { return (x == p.x) && (y == p.y); }
};
}  // namespace base
