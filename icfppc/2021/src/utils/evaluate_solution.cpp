#include "utils/evaluate_solution.h"

#include "best_score.h"
#include "evaluator.h"
#include "problem.h"
#include "settings.h"
#include "solution.h"

#include "common/solvers/ext/evaluate.h"

#include <iostream>

void EvaluateSolution(const std::string& solver_name) {
  BestScore best_score;
  unsigned total = 0;
  for (unsigned i = 1; i <= last_problem; ++i) {
    Evaluator::Result r0(true, best_score(i));
    auto r = solvers::ext::Evaluate<Evaluator, Problem, Solution>(
        std::to_string(i), solver_name);
    unsigned fscore = ceil(r.raw_points / r0.DScore());
    total += fscore;
    std::cout << "Problem " << std::to_string(1000 + i).substr(1) << "\t"
              << r.correct << "\t" << r.score << "\t" << r0.score << "\t"
              << r.raw_points << "\t" << fscore << std::endl;
  }
  std::cout << "Total = " << total << std::endl;
}
