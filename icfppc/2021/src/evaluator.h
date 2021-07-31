#pragma once

#include "common/solvers/evaluator.h"

#include <string>

class Problem;
class Solution;

class Evaluator : public solvers::Evaluator {
 public:
  static Result Apply(const Problem& p, const Solution& s);
  static double DScore(const Result& r);
};
