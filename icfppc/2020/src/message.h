#pragma once

#include "line.h"

#include <vector>

class Message {
 public:
  std::vector<Line> v;

  void Process();
  void Print();
};
