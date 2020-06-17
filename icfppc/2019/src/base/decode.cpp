#include "base/decode.h"

#include "base/action.h"
#include "base/point.h"
#include "base/solution.h"

#include "common/assert_exception.h"
#include "common/string/utils/split.h"

#include <string>

namespace base {
Point DecodePoint(const std::string& s) {
  Assert((s.size() >= 2) && (s[0] == '(') && (s.back() == ')'));
  size_t npos = s.find(',');
  Assert(npos != std::string::npos);
  int x = std::stoi(s.substr(1, npos - 1));
  int y = std::stoi(s.substr(npos + 1, s.size() - npos - 2));
  return {x, y};
}

ActionsList DecodeActionList(const std::string& s) {
  ActionsList al;
  for (unsigned i = 0; i < s.size(); ++i) {
    switch (s[i]) {
      case 'W':
        al.push_back(ActionType::MOVE_UP);
        break;
      case 'S':
        al.push_back(ActionType::MOVE_DOWN);
        break;
      case 'A':
        al.push_back(ActionType::MOVE_LEFT);
        break;
      case 'D':
        al.push_back(ActionType::MOVE_RIGHT);
        break;
      case 'Z':
        al.push_back(ActionType::DO_NOTHING);
        break;
      case 'E':
        al.push_back(ActionType::ROTATE_CLOCKWISE);
        break;
      case 'Q':
        al.push_back(ActionType::ROTATE_COUNTERCLOCKWISE);
        break;
      case 'B': {
        al.push_back(ActionType::ATTACH_MANIPULATOR);
        size_t npos = s.find(')', i++);
        Assert(npos != std::string::npos);
        Assert(s[i] == '(');
        Point p = DecodePoint(s.substr(i, npos - i + 1));
        i = npos;
        al.back().x = p.x;
        al.back().y = p.y;
      } break;
      case 'F':
        al.push_back(ActionType::ATTACH_FAST_WHEELS);
        break;
      case 'L':
        al.push_back(ActionType::USING_DRILL);
        break;
      case 'R':
        al.push_back(ActionType::SET_BEACON);
        break;
      case 'T': {
        al.push_back(ActionType::SHIFT);
        size_t npos = s.find(')', i++);
        Assert(npos != std::string::npos);
        Assert(s[i] == '(');
        Point p = DecodePoint(s.substr(i, npos - i + 1));
        i = npos;
        al.back().x = p.x;
        al.back().y = p.y;
      } break;
      case 'C':
        al.push_back(ActionType::CLONE);
        break;
      default:
        Assert(false);
        break;
    }
  }
  return al;
}

Solution DecodeSolution(const std::string& s) {
  Solution output;
  for (const std::string& ss : Split(s, '#')) {
    output.actions.push_back(DecodeActionList(ss));
  }
  return output;
}
}  // namespace base
