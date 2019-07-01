#include "base/map_core.h"

#include "base/point.h"
#include <cassert>

namespace base {
void MapCore::InitSize(int _xsize, int _ysize) {
  xsize = _xsize;
  ysize = _ysize;
}

unsigned MapCore::Size() const { return unsigned(xsize * ysize); }

unsigned MapCore::Index(int x, int y) const {
  assert(Inside(x, y));
  return x * ysize + y;
}

unsigned MapCore::Index(const Point& p) const { return Index(p.x, p.y); }

Point MapCore::GetPoint(unsigned index) const {
  return {int(index) / ysize, int(index) % ysize};
}

bool MapCore::Inside(int x, int y) const {
  return (0 <= x) && (x < xsize) && (0 <= y) && (y < ysize);
}

bool MapCore::Inside(const Point& p) const { return Inside(p.x, p.y); }
}  // namespace base
