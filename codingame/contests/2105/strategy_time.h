#pragma once

#include "settings.h"
#include "strategy.h"

#include <chrono>

class StrategyTime : public Strategy {
 public:
  using TBase = Strategy;

 protected:
  std::chrono::_V2::system_clock::time_point start_time;

 protected:
  void StartTurn() { start_time = std::chrono::high_resolution_clock::now(); }
  bool TimeToStop() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - start_time)
               .count() > MaxTimePerMove();
  }
};
