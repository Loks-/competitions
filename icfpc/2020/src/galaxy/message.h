#pragma once

#include "galaxy/line.h"

#include <vector>

namespace galaxy {
class Message {
 public:
  std::vector<Line> v;

  void Process();
  void Print();
};
}  // namespace galaxy
