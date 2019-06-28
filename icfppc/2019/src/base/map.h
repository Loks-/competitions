#pragma once

#include "base/booster_type.h"
#include "base/point.h"
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
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
  std::unordered_map<unsigned, BoosterType> boosters;
  std::unordered_set<unsigned> beacons;
  std::unordered_set<unsigned> codex;

 public:
  unsigned Index(const Point& p) const;
  Point GetPoint(unsigned index) const;

  BoosterType PickupItem(const Point& p);

  bool Inside(const Point& p) const;
  bool Obstacle(const Point& p) const;
  void Wrap(const Point& p);
  void Drill(const Point& p);

  void AddBeacon(const Point& p);
  bool CheckBeacon(const Point& p) const;
  bool CheckCodeX(const Point& p) const;

  bool Wrapped() const;
};
}  // namespace base
