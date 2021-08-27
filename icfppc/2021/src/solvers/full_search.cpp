#include "solvers/full_search.h"

#include "evaluator.h"

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace src_solvers;

void FullSearch::InitSearch(const Problem& p) {
  TBase::InitSearch(p);
  ShuffleCandidatesOrder();
  best_score = (1 << 30);
}

bool FullSearch::ForceStop() const {
  return (best_score == 0) || (t.GetSeconds() >= max_time_in_seconds) ||
         (cache.SegmentsMapSize() >= (1u << 23));
}

void FullSearch::Run() {
  if (ForceStop()) return;
  if (used_vertices.Size() == used_vertices.SetSize()) {
    Solution s{problem.Id(), solution};
    auto r = Evaluator::Apply(problem, s);
    if (r.correct && (r.score < best_score)) {
      best_score = r.score;
      best_solution = s;
    }
    return;
  }
  unsigned next_u = used_vertices.SetSize();
  if (used_vertices.Size() == 0) {
    unsigned max_degree = 0;
    for (unsigned u = 0; u < used_vertices.SetSize(); ++u) {
      auto d = problem.Figure().Edges(u).size();
      if (max_degree < d) {
        max_degree = d;
        next_u = u;
      }
    }
  } else {
    unsigned min_size = cache.MaxIndex() + 1;
    for (unsigned u = 0; u < used_vertices.SetSize(); ++u) {
      if (used_vertices.HasKey(u)) continue;
      auto csize = valid_candidates[u][valid_candidates_index[u]].size();
      if (min_size > csize) {
        min_size = csize;
        next_u = u;
      }
    }
    assert(min_size > 0);
  }
  assert(next_u < used_vertices.SetSize());
  auto& vc = valid_candidates[next_u][valid_candidates_index[next_u]];
  for (auto& p : vc) {
    AddPoint(next_u, p);
    if (!force_stop) Run();
    RemoveLastPoint();
    if (ForceStop()) break;
  }
}

Solution FullSearch::Solve(const Problem& p) {
  std::cout << "Running FullSearch::Solve for problem " << p.Id() << std::endl;
  t.Start();
  InitSearch(p);
  Run();
  std::cout << "\tBest score = " << int(best_score) << std::endl;
  return (best_score < (1 << 30)) ? best_solution : Solution();
}
