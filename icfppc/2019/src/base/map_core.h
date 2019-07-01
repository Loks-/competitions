#pragma once

#include "base/point.h"

namespace base {
class MapCore {
 protected:
  int xsize;
  int ysize;

 protected:
  void InitSize(int _xsize, int _ysize);

 public:
  unsigned Size() const;
  unsigned Index(int x, int y) const;
  unsigned Index(const Point& p) const;
  Point GetPoint(unsigned index) const;
  bool Inside(int x, int y) const;
  bool Inside(const Point& p) const;
};
}  // namespace base
