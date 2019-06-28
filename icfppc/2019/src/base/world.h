#pragma once

#include "base/boosters.h"
#include "base/map.h"
#include "base/worker.h"
#include <vector>

namespace base {
class Action;
class Solution;

class World {
 protected:
  Boosters boosters;
  Map map;
  int time;
  std::vector<Worker> workers;

 public:
  Boosters& GetBoosters();
  Map& GetMap();
  int GetTime() const;

  void Apply(unsigned worker_index, const Action& action);
  void Apply(const Solution& solution);
  bool Solved() const;
};
}  // namespace base
