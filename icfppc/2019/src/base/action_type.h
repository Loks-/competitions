#pragma once

namespace base {
enum class ActionType {
  MOVE_UP,
  MOVE_DOWN,
  MOVE_LEFT,
  MOVE_RIGHT,
  DO_NOTHING,
  ROTATE_CLOCKWISE,
  ROTATE_COUNTERCLOCKWISE,
  ATTACH_MANIPULATOR,
  ATTACH_FAST_WHEELS,
  USING_DRILL,
  SET_BEACON,
  SHIFT,
  CLONE,
  END
};
}  // namespace base
