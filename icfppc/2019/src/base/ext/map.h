#pragma once

#include "base/action.h"
#include "base/core/map.h"
#include "base/ext/event.h"
#include "base/point.h"
#include "common/graph/graph.h"
#include "common/unsigned_set.h"
#include <string>
#include <vector>

namespace base {
namespace ext {
class Map : public core::Map {
 protected:
  std::vector<bool> obstacles;
  UnsignedSet unwrapped;
  DirectedGraph gmove;
  std::vector<Event> events;

 protected:
  void ResetSize();
  void InitMap(const std::string& map_encoded);
  void InitGraph();

 public:
  void Init(const std::string& problem);

  bool Obstacle(const Point& p) const;
  void Wrap(const Point& p);
  void Drill(const Point& p);
  bool Wrapped() const;

  Action Move(unsigned from, unsigned to) const;
  Action Move(const Point& from, const Point& to) const;

  const std::vector<unsigned>& GEdges(unsigned from) const;
  const UnsignedSet& Unwrapped() const;
};
}  // namespace ext
}  // namespace base
