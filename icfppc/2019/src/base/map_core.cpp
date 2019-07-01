#include "base/map_core.h"

#include "base/booster_type.h"
#include "base/point.h"
#include "common/assert_exception.h"
#include <cassert>

namespace base {
void MapCore::InitSize(int _xsize, int _ysize) {
  xsize = _xsize;
  ysize = _ysize;
}

void MapCore::AddBooster(const Point& p, BoosterType type) {
  Assert(Inside(p));
  unsigned index = Index(p);
  Assert(boosters.find(index) == boosters.end());
  boosters[index] = type;
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

BoosterType MapCore::PickupItem(const Point& p) {
  assert(Inside(p));
  auto it = boosters.find(Index(p));
  if (it == boosters.end()) return BoosterType::NONE;
  auto type = it->second;
  boosters.erase(it);
  return type;
}

void MapCore::AddBeacon(const Point& p) {
  assert(Inside(p));
  beacons.insert(Index(p));
}

bool MapCore::CheckBeacon(const Point& p) const {
  assert(Inside(p));
  return beacons.find(Index(p)) != beacons.end();
}

bool MapCore::CheckCodeX(const Point& p) const {
  assert(Inside(p));
  return codex.find(Index(p)) != codex.end();
}
}  // namespace base
