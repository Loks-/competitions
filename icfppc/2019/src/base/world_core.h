#pragma once

#include "base/boosters.h"
#include "base/worker.h"
#include <string>
#include <vector>

namespace base {
class Action;
class Solution;

template <class TTMap>
class WorldCore {
 public:
  using TMap = TTMap;
  using TSelf = WorldCore<TMap>;
  using TWorker = Worker<TSelf>;

 protected:
  Boosters boosters;
  TMap map;
  int time;
  std::vector<TWorker> workers;

 public:
  void Init(const std::string& problem);

  Boosters& GetBoosters();
  TMap& GetMap();
  int GetTime() const;

  void Apply(unsigned worker_index, const Action& action);
  void Apply(const Solution& solution);
  bool Solved() const;
};
}  // namespace base
