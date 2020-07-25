#include "tournament/location_with_speed.h"

#include "tournament/acceleration.h"

#include "common/numeric/utils/sign.h"

#include <cstdlib>

namespace tournament {
LocationWithSpeed::LocationWithSpeed() : Location(), vx(0), vy(0) {}

LocationWithSpeed::LocationWithSpeed(int _x, int _y, int _vx, int _vy)
    : Location(_x, _y), vx(_vx), vy(_vy) {}

LocationWithSpeed LocationWithSpeed::Next() const { return Next({0, 0}); }

LocationWithSpeed LocationWithSpeed::Next(const Acceleration& a) const {
  int ax = -a.ax, ay = -a.ay;
  int mx = abs(x), my = abs(y);
  if (mx >= my) ax += Sign(x);
  if (my >= mx) ay += Sign(y);
  int nvx = vx + ax, nvy = vy + ay;
  return LocationWithSpeed(x + nvx, y + nvy, nvx, nvy);
}

LocationWithSpeed LocationWithSpeed::Prev() const { return Prev({0, 0}); }

LocationWithSpeed LocationWithSpeed::Prev(const Acceleration& a) const {
  int px = x - vx, py = y - vy;
  int ax = -a.ax, ay = -a.ay;
  int mx = abs(px), my = abs(py);
  if (mx >= my) ax += Sign(x);
  if (my >= mx) ay += Sign(y);
  return LocationWithSpeed(px, py, vx - ax, vy - ay);
}
}  // namespace tournament
