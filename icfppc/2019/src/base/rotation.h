#pragma once

#include "base/action_type.h"
#include "base/point.h"
#include "base/rotation_type.h"

namespace base {
class Rotation {
 protected:
  RotationType type;

 public:
  Rotation(RotationType type);
  Rotation(ActionType type);

  RotationType GetType() const;
  ActionType GetAction() const;
  void Set(ActionType type);

  Point operator()(const Point& p) const;
};
}  // namespace base
