#include "base/action.h"

#include "base/action_type.h"

namespace base {
Action::Action() : type(ActionType::END), x(0), y(0) {}
Action::Action(ActionType _type) : type(_type), x(0), y(0) {}
Action::Action(ActionType _type, int _x, int _y) : type(_type), x(_x), y(_y) {}
}  // namespace base
