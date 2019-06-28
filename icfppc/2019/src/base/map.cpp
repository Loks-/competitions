#include "base/map.h"

#include "base/booster_type.h"
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

BoosterType Map::PickupItem(const Point& p) {
  assert(Inside(p));
  auto it = boosters.find(Index(p));
  if (it == boosters.end()) return BoosterType::NONE;
  auto type = it->second;
  boosters.erase(it);
  return type;
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

void Map::Drill(const Point& p) {
  assert(Inside(p));
  map[Index(p)] &= ~OBSTACLE;
}

void Map::AddBeacon(const Point& p) {
  assert(Inside(p));
  beacons.insert(Index(p));
}

bool Map::CheckBeacon(const Point& p) const {
  assert(Inside(p));
  return beacons.find(Index(p)) != beacons.end();
}

}  // namespace base
