#include "tournament/action.h"

#include "common/base.h"

std::string Action::EncodePair(int x, int y) {
  return "ap ap vec " + std::to_string(x) + " " + std::to_string(y);
}

std::string Action::Encode() const {
  switch (type) {
    case ACCELERATE:
      return "( 0 , " + std::to_string(ship_id) + " , " +
             EncodePair(acceleration.ax, acceleration.ay) + " )";
    case DETONATE:
      return "( 1 , " + std::to_string(ship_id) + " )";
    case SHOOT:
      return "( 2 , " + std::to_string(ship_id) + " , " +
             EncodePair(target.x, target.y) + " , " + std::to_string(power) +
             " )";
    case SPLIT:
      return "( 3 , " + std::to_string(ship_id) + " , ( " +
             std::to_string(matter.fuel) + " , " +
             std::to_string(matter.lasers) + " , " +
             std::to_string(matter.radiators) + " , " +
             std::to_string(matter.engines) + " ) )";
    default:
      assert(false);
      return "";
  }
}

std::string Encode(const Actions& actions) {
  std::string s = " ( ";
  for (unsigned i = 0; i < actions.size(); ++i) {
    s += ((i > 0) ? ", " : "") + actions[i].Encode() + " ";
  }
  s += ")";
  return s;
}
