#pragma once

#include "base/action.h"
#include "base/manipulators.h"
#include "base/point.h"
#include <vector>

namespace base {
template <class TTWorld>
class Worker {
 public:
  using TWorld = TTWorld;
  using TMap = typename TWorld::TMap;

 protected:
  Point p;
  TWorld* pworld;
  Manipulators manipulators;
  int time_fast_wheels;
  int time_drill;

 public:
  Worker(const Point& _p, TWorld& world);

  const Point& Location() const;
  void Wrap();
  void PickupItem();
  void operator()(const Action& action);
};
}  // namespace base
