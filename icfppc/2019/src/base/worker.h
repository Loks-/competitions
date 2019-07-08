#pragma once

#include "base/action.h"
#include "base/manipulators.h"
#include "base/point.h"
#include <vector>

namespace base {
template <class TMap>
class Worker {
 protected:
  Point p;
  TMap& map;
  Manipulators manipulators;
  int time_fast_wheels;
  int time_drill;

 public:
  Worker(const Point& _p, TMap& _map);

  const Point& Location() const;
  void Wrap();
  void PickupItem();
  void operator()(const Action& action);
};
}  // namespace base
