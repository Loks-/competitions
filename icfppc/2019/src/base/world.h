#pragma once

#include "base/boosters.h"
#include "base/map.h"
#include "base/worker.h"
#include <string>
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
  void Init(const std::string& problem);

  Boosters& GetBoosters();
  Map& GetMap();
  int GetTime() const;

  void Apply(unsigned worker_index, const Action& action);
  void Apply(const Solution& solution);
  bool Solved() const;
};
}  // namespace base
