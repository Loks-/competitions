#pragma once

#include "base/action_type.h"
#include "base/point.h"

namespace base {
class Direction {
 public:
  unsigned direction;

 public:
  Direction(unsigned d);
  Direction(ActionType type);

  ActionType Get() const;
  void Set(ActionType type);

  Point operator()(const Point& p) const;
};
}  // namespace base
