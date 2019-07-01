#pragma once

#include "base/action.h"
#include "base/manipulators.h"
#include "base/point.h"
#include <vector>

namespace base {
template <class TMap>
class WorldCore;
class Map;
using World = WorldCore<Map>;

class Worker {
 protected:
  Point p;
  World* pworld;
  Manipulators manipulators;
  int time_fast_wheels;
  int time_drill;

 public:
  Worker(const Point& _p, World& world);

  const Point& Location() const;
  void Wrap();
  void PickupItem();
  void operator()(const Action& action);
};
}  // namespace base
