#include "base/worker.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/manipulator.h"
#include "base/point.h"
#include "base/rotation.h"
#include "base/world.h"
#include <cassert>

namespace base {
Worker::Worker(const Point& _p, World& world) : p(_p), pworld(&world) {
  time_fast_move = -1;
  time_drill = -1;
}

void Worker::Wrap() {
  assert(pworld);
  manipulators.Wrap(p, pworld->GetMap());
}

void Worker::operator()(const Action& action) {
  switch (action.type) {
    case ActionType::MOVE_UP:
    case ActionType::MOVE_DOWN:
    case ActionType::MOVE_LEFT:
    case ActionType::MOVE_RIGHT:
      assert(false);
      break;
    case ActionType::DO_NOTHING:
      break;
    case ActionType::ROTATE_CLOCKWISE:
    case ActionType::ROTATE_COUNTERCLOCKWISE: {
      Rotation r(action.type);
      manipulators.Rotate(r.GetType());
      Wrap();
    } break;
    case ActionType::ATTACH_MANIPULATOR:
      assert(false);
      break;
    case ActionType::ATTACH_FAST_WHEELS:
      assert(false);
      break;
    case ActionType::USING_DRILL:
      assert(false);
      break;
    case ActionType::SET_BEACON:
      assert(false);
      break;
    case ActionType::SHIFT:
      assert(false);
      break;
    default:
      assert(false);
  }
}
}  // namespace base
