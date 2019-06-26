#include "base/map.h"

#include "base/point.h"
#include <cassert>

namespace base {
unsigned Map::Index(const Point& p) const {
  assert(Inside(p));
  return p.x + ysize + p.y;
}

Point Map::GetPoint(unsigned index) const {
  return {int(index) / ysize, int(index) % ysize};
}

bool Map::Inside(const Point& p) const {
  return (0 <= p.x) && (p.x < xsize) && (0 <= p.y) && (p.y < ysize);
}

bool Map::Obstacle(const Point& p) const {
  return !Inside(p) || (map[Index(p)] & OBSTACLE);
}

void Map::Wrap(const Point& p) {
  assert(Inside(p));
  map[Index(p)] |= WRAPPED;
}
}  // namespace base
