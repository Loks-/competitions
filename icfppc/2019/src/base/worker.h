#pragma once

#include "base/action.h"
#include "base/manipulators.h"
#include "base/point.h"
#include <vector>

namespace base {
class World;

class Worker {
 protected:
  Point p;
  World* pworld;
  Manipulators manipulators;
  int time_fast_move;
  int time_drill;

 public:
  Worker(const Point& _p, World& world);

  void Wrap();
  void operator()(const Action& action);
};
}  // namespace base
