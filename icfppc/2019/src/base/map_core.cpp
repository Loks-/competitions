#include "base/map_core.h"

#include "base/booster_type.h"
#include "base/decode.h"
#include "base/point.h"
#include "common/assert_exception.h"
#include "common/string/split.h"
#include <cassert>
#include <string>

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

void MapCore::InitBoosters(const std::string& boosters_encoded) {
  boosters.clear();
  beacons.clear();
  codex.clear();
  for (auto& boost_desc : Split(boosters_encoded, ';')) {
    Assert(boost_desc.size() >= 2);
    Point p = DecodePoint(boost_desc.substr(1));
    switch (boost_desc[0]) {
      case 'B':
        AddBooster(p, BoosterType::EXTENSION);
        break;
      case 'F':
        AddBooster(p, BoosterType::FAST_WHEELS);
        break;
      case 'L':
        AddBooster(p, BoosterType::DRILL);
        break;
      case 'X':
        codex.insert(Index(p));
        break;
      case 'R':
        AddBooster(p, BoosterType::TELEPORT);
        break;
      case 'C':
        AddBooster(p, BoosterType::CLONING);
        break;
      default:
        Assert(false, "Unknown item the in problem description.");
    }
  }
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
