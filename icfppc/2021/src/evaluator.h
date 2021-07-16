#pragma once

#include "common/solvers/evaluator.h"

#include <string>

class Problem;
class Solution;

class Evaluator : public solvers::Evaluator {
 public:
  Result operator()(const Problem& p, const Solution& s) const;

  Result operator()(const std::string& id, const std::string& problem_filename,
                    const std::string& solution_filename) const;

  Result operator()(const std::string& problem_filename,
                    const std::string& solution_filename) const {
    return operator()("", problem_filename, solution_filename);
  }
};
