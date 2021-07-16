#include "utils/evaluate_solution.h"

#include "evaluator.h"
#include "settings.h"

#include <iostream>

void EvaluateSolution(const std::string& solution_name) {
  const auto dir_problems = path_to_root + "problems/";
  const auto dir_solutions = path_to_root + "solutions/" + solution_name + "/";
  Evaluator e;
  for (unsigned i = 1; i <= last_problem; ++i) {
    auto r = e(ProblemFilename(i), SolutionFilename(i, solution_name));
    std::cout << "Problem " << std::to_string(1000 + i).substr(1) << "\t"
              << r.correct << "\t" << r.score << std::endl;
  }
}
