#include "solvers/mctp.h"

#include "evaluator.h"

#include "common/timer.h"

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace src_solvers;

void MCTP::InitSearch(const Problem& p) { TBase::InitSearch(p); }

void MCTP::Run() {
  TBase::Run();
  double dscore = 0.;
  if (used_vertices.Size() == problem.Figure().Size()) {
    Solution s{problem.Id(), solution};
    auto r = Evaluator::Apply(problem, s);
    if (r.DScore() > best_score) {
      //   std::cout << "New best solution: " << r.score << "\t" << nruns
      //             << std::endl;
      best_score = r.DScore();
      best_solution = s;
      max_score = best_score;
    }
  }
  UpdateStat(dscore);
}

Solution MCTP::Solve(const Problem& p) {
  std::cout << "Running MCTP::Solve for problem " << p.Id() << std::endl;
  Timer t;
  InitSearch(p);
  for (; (best_score < 1) && (t.GetSeconds() < max_time_in_seconds);) Run();
  std::cout << "\tBest score = " << best_score << std::endl;
  return best_solution;
}
