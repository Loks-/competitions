#pragma once

#include "base/action.h"
#include "base/map_core.h"
#include "base/point.h"
#include "common/graph/graph.h"
#include "common/unsigned_set.h"
#include <string>
#include <vector>

namespace base {
namespace ext {
class Map : public MapCore {
 protected:
  std::vector<bool> obstacles;
  UnsignedSet unwrapped;
  DirectedGraph gmove;

 protected:
  void InitSize(int _xsize, int _ysize);
  void InitMap(const std::string& desc);
  void InitGraph();

 public:
  void Init(const std::string& problem);

  bool Obstacle(const Point& p) const;
  void Wrap(const Point& p);
  void Drill(const Point& p);
  bool Wrapped() const;

  Action Move(unsigned from, unsigned to) const;
  Action Move(const Point& from, const Point& to) const;
};
}  // namespace ext
}  // namespace base
