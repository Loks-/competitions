#pragma once

#include "galaxy/expression.h"

#include <vector>

namespace galaxy {
class Line {
 public:
  std::vector<Expression> v;

  void Print();
};
}  // namespace galaxy
