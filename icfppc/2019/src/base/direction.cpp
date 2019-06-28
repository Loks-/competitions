#include "base/direction.h"

#include "base/action_type.h"
#include "base/point.h"
#include <cassert>
#include <vector>

namespace base {
namespace {
const std::vector<ActionType> vd{ActionType::MOVE_RIGHT, ActionType::MOVE_UP,
                                 ActionType::MOVE_LEFT, ActionType::MOVE_DOWN};
const std::vector<int> dx{1, 0, -1, 0};
const std::vector<int> dy{0, 1, 0, -1};
}  // namespace
Direction::Direction(unsigned d) : direction(d % 4) {}

Direction::Direction(ActionType type) { Set(type); }

ActionType Direction::Get() const { return vd[direction]; }

void Direction::Set(ActionType t) {
  switch (t) {
    case ActionType::MOVE_RIGHT:
      direction = 0;
      break;
    case ActionType::MOVE_UP:
      direction = 1;
      break;
    case ActionType::MOVE_LEFT:
      direction = 2;
      break;
    case ActionType::MOVE_DOWN:
      direction = 3;
      break;
    default:
      assert(false);
  }
}

Point Direction::operator()(const Point& p) const {
  return {p.x + dx[direction], p.y + dy[direction]};
}
}  // namespace base
