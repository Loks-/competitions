#pragma once

#include "base/booster_type.h"
#include "base/point.h"
#include <unordered_map>
#include <unordered_set>

namespace base {
class MapCore {
 protected:
  int xsize;
  int ysize;

  std::unordered_map<unsigned, BoosterType> boosters;
  std::unordered_set<unsigned> beacons;
  std::unordered_set<unsigned> codex;

 protected:
  void InitSize(int _xsize, int _ysize);
  void AddBooster(const Point& p, BoosterType type);
  void InitBoosters(const std::string& boosters_encoded);

 public:
  unsigned Size() const;
  unsigned Index(int x, int y) const;
  unsigned Index(const Point& p) const;
  Point GetPoint(unsigned index) const;
  bool Inside(int x, int y) const;
  bool Inside(const Point& p) const;

  BoosterType PickupItem(const Point& p);
  void AddBeacon(const Point& p);
  bool CheckBeacon(const Point& p) const;
  bool CheckCodeX(const Point& p) const;
};
}  // namespace base
