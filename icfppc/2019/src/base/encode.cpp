#include "base/encode.h"

#include "base/action.h"
#include "base/solution.h"
#include <cassert>
#include <ostream>
#include <sstream>
#include <string>

namespace base {
std::ostream& operator<<(std::ostream& os, const Action& a) {
  switch (a.type) {
    case ActionType::MOVE_UP:
      return os << "W";
    case ActionType::MOVE_DOWN:
      return os << "S";
    case ActionType::MOVE_LEFT:
      return os << "A";
    case ActionType::MOVE_RIGHT:
      return os << "D";
    case ActionType::DO_NOTHING:
      return os << "Z";
    case ActionType::ROTATE_CLOCKWISE:
      return os << "E";
    case ActionType::ROTATE_COUNTERCLOCKWISE:
      return os << "Q";
    case ActionType::ATTACH_MANIPULATOR:
      return os << "B(" << a.x << "," << a.y << ")";
    case ActionType::ATTACH_FAST_WHEELS:
      return os << "F";
    case ActionType::USING_DRILL:
      return os << "L";
    case ActionType::SET_BEACON:
      return os << "R";
    case ActionType::SHIFT:
      return os << "T(" << a.x << "," << a.y << ")";
    case ActionType::CLONE:
      return os << "C";
  }
  assert(false);
  return os;
}

std::ostream& operator<<(std::ostream& os, const ActionsList& al) {
  for (auto a : al) os << a;
  return os;
}

std::ostream& operator<<(std::ostream& os, const Solution& s) {
  assert(s.actions.size() > 0);
  os << s.actions[0];
  for (unsigned i = 1; i < s.actions.size(); ++i) os << "#" << s.actions[i];
  return os;
}

std::string Encode(const Action& a) {
  std::ostringstream ss;
  ss << a;
  return ss.str();
}

std::string Encode(const ActionsList& a) {
  std::ostringstream ss;
  ss << a;
  return ss.str();
}

std::string Encode(const Solution& a) {
  std::ostringstream ss;
  ss << a;
  return ss.str();
}
}  // namespace base
