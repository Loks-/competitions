#pragma once

#include "base/action.h"
#include "base/boosters.h"
#include "base/map.h"
#include "base/worker.h"
#include <vector>

namespace base {
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
};
}  // namespace base
