#include "base/map_core.h"

#include "base/booster_type.h"
#include "base/decode.h"
#include "base/point.h"
#include "common/assert_exception.h"
#include "common/string/split.h"
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

namespace base {
void MapCore::InitSize(int _xsize, int _ysize) {
  xsize = _xsize;
  ysize = _ysize;
}

void MapCore::InitCore(const std::string& map_encoded,
                       std::vector<std::vector<int>>& output_vvy) {
  output_vvy.clear();
  int xs = 0, ys = 0;
  std::vector<Point> v;
  for (const std::string& scontour : Split(map_encoded, ';')) {
    v.clear();
    auto vst = Split(scontour, ',');
    Assert((vst.size() % 2) == 0);
    std::vector<std::string> vs;
    for (unsigned i = 0; i < vst.size(); i += 2)
      vs.push_back(vst[i] + "," + vst[i + 1]);
    for (const std::string& st : vs) {
      Point p = DecodePoint(st);
      xs = std::max(p.x, xs);
      ys = std::max(p.y, ys);
      v.push_back(p);
    }
    Assert(v.size() > 1);
    v.push_back(v[0]);
    if (output_vvy.size() < xs) output_vvy.resize(xs);
    for (size_t i = 1; i < v.size(); ++i) {
      if (v[i - 1].y == v[i].y) {
        int x1 = std::min(v[i - 1].x, v[i].x);
        int x2 = std::max(v[i - 1].x, v[i].x);
        for (int x = x1; x < x2; ++x) output_vvy[x].push_back(v[i].y);
      } else {
        Assert(v[i - 1].x == v[i].x);
      }
    }
  }
  InitSize(xs, ys);
  for (int x = 0; x < xs; ++x) {
    auto& vy = output_vvy[x];
    std::sort(vy.begin(), vy.end());
    Assert((vy.size() % 2) == 0);
    vy.push_back(ys);
    vy.push_back(ys);
  }
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
