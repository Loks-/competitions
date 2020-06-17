#include "base/eval/map.h"

#include "base/point.h"

#include "common/assert_exception.h"
#include "common/string/utils/split.h"

#include <cassert>
#include <string>

namespace base {
namespace eval {
void Map::ResetSize() {
  map.clear();
  map.resize(Size());
}

void Map::InitMap(const std::string& map_encoded) {
  std::vector<std::vector<int>> vvy;
  InitCore(map_encoded, vvy);
  ResetSize();
  for (int x = 0; x < xsize; ++x) {
    auto& vy = vvy[x];
    int y = 0;
    for (unsigned i = 0; i < vy.size(); i += 2) {
      for (; y < vy[i]; ++y) map[Index(x, y)] |= OBSTACLE;
      y = vy[i + 1];
    }
  }
}

void Map::Init(const std::string& problem) {
  auto vs = Split(problem, '#');
  Assert(vs.size() == 4);
  InitMap(vs[2].empty() ? vs[0] : vs[0] + ";" + vs[2]);
  InitItems(vs[3]);
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

bool Map::Wrapped() const {
  for (unsigned x : map) {
    if ((x & (OBSTACLE | WRAPPED)) == 0) return false;
  }
  return true;
}
}  // namespace eval
}  // namespace base
