#include "solvers/mctp_one.h"

#include "common/timer.h"

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace src_solvers;

void MCTPOne::InitSearch(const Problem& p) {
  TBase::InitSearch(p);
  best_score = -double(unused_vertices.SetSize());
}

void MCTPOne::Run() {
  TBase::Run();
  double dscore = -double(unused_vertices.Size());
  UpdateStat(dscore);
  best_score = std::max(best_score, dscore);
  if (best_score >= 0) best_solution = Solution(problem.Id(), solution);
}

Solution MCTPOne::Solve(const Problem& p) {
  std::cout << "Running MCTPOne::Solve for problem " << p.Id() << std::endl;
  Timer t;
  InitSearch(p);
  for (; (best_score < 0) && (t.GetSeconds() < max_time_in_seconds) &&
         (cache.SegmentsMapSize() < (1u << 23));)
    Run();
  std::cout << "\tBest score = " << best_score << std::endl;
  return (best_score >= 0) ? best_solution : Solution();
}
