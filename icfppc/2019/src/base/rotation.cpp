#include "base/rotation.h"

#include "base/action_type.h"
#include "base/point.h"
#include "base/rotation_type.h"
#include <cassert>

namespace base {
Rotation::Rotation(RotationType _type) : type(_type) {}
Rotation::Rotation(ActionType _type) { Set(_type); }

RotationType Rotation::GetType() const { return type; }

ActionType Rotation::GetAction() const {
  switch (type) {
    case RotationType::CLOCKWISE:
      return ActionType::ROTATE_CLOCKWISE;
    case RotationType::COUNTER_CLOCKWISE:
      return ActionType::ROTATE_COUNTERCLOCKWISE;
  }
  assert(false);
  return ActionType::END;
}

void Rotation::Set(ActionType _type) {
  switch (_type) {
    case ActionType::ROTATE_CLOCKWISE:
      type = RotationType::CLOCKWISE;
      break;
    case ActionType::ROTATE_COUNTERCLOCKWISE:
      type = RotationType::COUNTER_CLOCKWISE;
      break;
    default:
      assert(false);
  }
}

Point Rotation::operator()(const Point& p) const {
  return (type == RotationType::CLOCKWISE) ? Point{p.y, -p.x}
                                           : Point{-p.y, p.x};
}
}  // namespace base
