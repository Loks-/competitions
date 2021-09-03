#include "solvers/full_search3.h"

#include "evaluator.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace src_solvers;

void FullSearch3::InitSearch(const Problem& p) {
  TBase::InitSearch(p);
  ShuffleCandidatesOrder();
  best_score = (1 << 30);
}

bool FullSearch3::ForceStop() const {
  return (best_score == 0) || (t.GetSeconds() >= max_time_in_seconds) ||
         (cache.SegmentsMapSize() >= (1u << 23));
}

void FullSearch3::Run() {
  if (ForceStop()) return;
  unsigned next_u = unused_vertices.SetSize();
  unsigned max_order = 0;
  unsigned min_size = 0;
  for (unsigned u : unused_vertices.List()) {
    unsigned order = remaining_order[u];
    if (order == 0) continue;
    unsigned csize = valid_candidates[u][valid_candidates_index[u]].size();
    if (csize == 1) order = unused_vertices.SetSize();  // Boost priority
    if ((max_order < order) || ((max_order == order) && (min_size > csize))) {
      next_u = u;
      max_order = order;
      min_size = csize;
    }
  }
  if (next_u < unused_vertices.SetSize()) {
    auto& vc = valid_candidates[next_u][valid_candidates_index[next_u]];
    for (auto& p : vc) {
      AddPointFDC(next_u, p);
      if (!force_stop) Run();
      RemoveLastPointFDC();
      if (ForceStop()) break;
    }
  } else {
    ZeroOrderOptimization();
    Solution s{problem.Id(), solution};
    auto r = Evaluator::Apply(problem, s);
    assert(r.correct);
    if (r.correct && (r.score < best_score)) {
      best_score = r.score;
      best_solution = s;
    }
  }
}

Solution FullSearch3::Solve(const Problem& p) {
  std::cout << "Running FullSearch3::Solve for problem " << p.Id() << std::endl;
  t.Start();
  InitSearch(p);
  Run();
  std::cout << "\tBest score = " << int(best_score) << std::endl;
  return (best_score < (1 << 30)) ? best_solution : Solution();
}
