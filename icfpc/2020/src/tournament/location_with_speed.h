#pragma once

#include "tournament/location.h"

namespace tournament {
class Acceleration;

class LocationWithSpeed : public Location {
 public:
  int vx, vy;

  LocationWithSpeed();
  LocationWithSpeed(int _x, int _y, int _vx, int _vy);

  LocationWithSpeed Next() const;
  LocationWithSpeed Next(const Acceleration& a) const;
  LocationWithSpeed Prev() const;
  LocationWithSpeed Prev(const Acceleration& a) const;
};
}  // namespace tournament
