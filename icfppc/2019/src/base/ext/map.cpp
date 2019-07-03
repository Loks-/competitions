#include "base/ext/map.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/direction.h"
#include "base/point.h"
#include "common/assert_exception.h"
#include "common/string/split.h"
#include "common/unsigned_set.h"
#include <cassert>
#include <string>

namespace base {
namespace ext {
void Map::ResetSize() {
  obstacles.clear();
  unsigned size = Size();
  obstacles.resize(size);
  unwrapped.Resize(size);
}

void Map::InitMap(const std::string& map_encoded) {
  std::vector<std::vector<int>> vvy;
  InitCore(map_encoded, vvy);
  ResetSize();
  for (int x = 0; x < xsize; ++x) {
    auto& vy = vvy[x];
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
  InitItems(vs[3]);
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

const std::vector<unsigned>& Map::GEdges(unsigned from) const {
  return gmove.Edges(from);
}

const UnsignedSet& Map::Unwrapped() const { return unwrapped; }
}  // namespace ext
}  // namespace base
