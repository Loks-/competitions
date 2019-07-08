#pragma once

#include "base/worker.h"
#include <string>
#include <vector>

namespace base {
class Action;
class Solution;

namespace core {
template <class TMap>
class World {
 public:
  using TWorker = Worker<TMap>;

 protected:
  TMap map;
  std::vector<TWorker> workers;

 public:
  void Init(const std::string& problem);

  TMap& GetMap();
  int GetTime() const;

  void AdjustTime(int dt);

  void Apply(unsigned worker_index, const Action& action);
  void Apply(const Solution& solution);
  bool Solved() const;
};
}  // namespace core
}  // namespace base
