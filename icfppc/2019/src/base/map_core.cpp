#include "base/map_core.h"

#include "base/point.h"
#include <cassert>

namespace base {
void MapCore::InitSize(int _xsize, int _ysize) {
  xsize = _xsize;
  ysize = _ysize;
}

unsigned MapCore::Size() const { return unsigned(xsize * ysize); }

unsigned MapCore::Index(const Point& p) const {
  assert(Inside(p));
  return p.x * ysize + p.y;
}

Point MapCore::GetPoint(unsigned index) const {
  return {int(index) / ysize, int(index) % ysize};
}

bool MapCore::Inside(const Point& p) const {
  return (0 <= p.x) && (p.x < xsize) && (0 <= p.y) && (p.y < ysize);
}
}  // namespace base
