#pragma once

#include "expression.h"

#include <vector>

class Line {
 public:
  std::vector<Expression> v;

  void Print();
};
