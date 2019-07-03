#pragma once

#include "base/booster_type.h"
#include "base/boosters.h"
#include "base/point.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace base {
namespace core {
class Map {
 protected:
  int xsize;
  int ysize;

  std::unordered_map<unsigned, BoosterType> items;
  std::unordered_set<unsigned> beacons;
  std::unordered_set<unsigned> codex;

  Boosters boosters;

 protected:
  void InitSize(int _xsize, int _ysize);
  void InitCore(const std::string& map_encoded,
                std::vector<std::vector<int>>& output_vvy);
  void AddItem(const Point& p, BoosterType type);
  void InitItems(const std::string& items_encoded);

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

  Boosters& GetBoosters();
};
}  // namespace core
}  // namespace base
