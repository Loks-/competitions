#pragma once

#include "base/booster_type.h"
#include "base/point.h"
#include "common/unsigned_set.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace base {
namespace ext {
class Map {
 protected:
  int xsize;
  int ysize;
  std::vector<bool> obstacles;
  UnsignedSet unwrapped;

  std::unordered_map<unsigned, BoosterType> boosters;
  std::unordered_set<unsigned> beacons;
  std::unordered_set<unsigned> codex;

 protected:
  void InitMap(const std::string& desc);
  void AddBooster(const Point& p, BoosterType type);

 public:
  void Init(const std::string& problem);

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
}  // namespace ext
}  // namespace base
