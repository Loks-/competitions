#include "base/ext/map.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/booster_type.h"
#include "base/decode.h"
#include "base/direction.h"
#include "base/point.h"
#include "common/assert_exception.h"
#include "common/string/split.h"
#include "common/unsigned_set.h"
#include <algorithm>
#include <cassert>
#include <string>

namespace base {
namespace ext {
void Map::InitSize(int _xsize, int _ysize) {
  MapCore::InitSize(_xsize, ysize);
  obstacles.clear();
  unsigned size = Size();
  obstacles.resize(size);
  unwrapped.Resize(size);
}

void Map::InitMap(const std::string& desc) {
  int xs = 0, ys = 0;
  std::vector<Point> v;
  std::vector<std::vector<int>> vvy;
  for (const std::string& scontour : Split(desc, ';')) {
    v.clear();
    auto vst = Split(scontour, ',');
    Assert((vst.size() % 2) == 0);
    std::vector<std::string> vs;
    for (unsigned i = 0; i < vst.size(); i += 2)
      vs.emplace_back(vst[i] + "," + vst[i + 1]);
    for (const std::string& st : vs) {
      Point p = DecodePoint(st);
      xs = std::max(p.x, xs);
      ys = std::max(p.y, ys);
      v.emplace_back(p);
    }
    Assert(v.size() > 1);
    v.push_back(v[0]);
    if (vvy.size() < xsize) vvy.resize(xsize);
    for (size_t i = 1; i < v.size(); ++i) {
      if (v[i - 1].y == v[i].y) {
        int x1 = std::min(v[i - 1].x, v[i].x);
        int x2 = std::max(v[i - 1].x, v[i].x);
        for (int x = x1; x < x2; ++x) vvy[x].push_back(v[i].y);
      } else {
        Assert(v[i - 1].x == v[i].x);
      }
    }
  }
  InitSize(xs, ys);
  for (int x = 0; x < xsize; ++x) {
    auto& vy = vvy[x];
    std::sort(vy.begin(), vy.end());
    Assert((vy.size() % 2) == 0);
    vy.push_back(ysize);
    vy.push_back(ysize);
    int y = 0;
    for (unsigned i = 0; i < vy.size(); i += 2) {
      for (; y < vy[i]; ++y) obstacles[Index(x, y)] = true;
      y = vy[i + 1];
    }
  }
}

void Map::InitGraph() {
  unsigned size = Size();
  gmove.Clear();
  gmove.Resize(size);
  for (int x = 0; x < xsize; ++x) {
    for (int y = 0; y < ysize; ++y) {
      unsigned index = Index(x, y);
      Point p{x, y};
      if (Obstacle(p)) continue;
      for (unsigned d = 0; d < 4; ++d) {
        Point pd = Direction(d)(p);
        if (!Obstacle(pd)) gmove.AddEdge(index, Index(pd));
      }
    }
  }
}

void Map::Init(const std::string& problem) {
  auto vs = Split(problem, '#');
  Assert(vs.size() == 4);
  InitMap(vs[2].empty() ? vs[0] : vs[0] + ";" + vs[2]);
  InitGraph();
  InitBoosters(vs[3]);
}

bool Map::Obstacle(const Point& p) const {
  return !Inside(p) || obstacles[Index(p)];
}

void Map::Wrap(const Point& p) {
  assert(Inside(p));
  unwrapped.Remove(Index(p));
}

void Map::Drill(const Point& p) {
  assert(Inside(p));
  obstacles[Index(p)] = false;
}

bool Map::Wrapped() const { return unwrapped.Empty(); }

Action Map::Move(unsigned from, unsigned to) const {
  return Move(GetPoint(from), GetPoint(to));
}

Action Map::Move(const Point& from, const Point& to) const {
  if (to.x == from.x) {
    if (to.y == from.y + 1) return ActionType::MOVE_UP;
    if (to.y == from.y) return ActionType::DO_NOTHING;
    if (to.y == from.y - 1) return ActionType::MOVE_DOWN;
  } else if (to.y == from.y) {
    if (to.x == from.x + 1) return ActionType::MOVE_RIGHT;
    if (to.x == from.x - 1) return ActionType::MOVE_LEFT;
  }
  assert(CheckBeacon(to));
  return Action(ActionType::SHIFT, to.x, to.y);
}
}  // namespace ext
}  // namespace base
