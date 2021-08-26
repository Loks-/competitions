#include "solvers/mctp_vertices.h"

#include "evaluator.h"

#include "common/timer.h"

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace src_solvers;

void MCTPVertices::InitSearch(const Problem& p) {
  TBase::InitSearch(p);
  best_score = -double(problem.Figure().Size());
  covered_vertices.Resize(p.Hole().Size());
  correct_last_run = false;
}

double MCTPVertices::DScore() const {
  if (used_vertices.Size() < problem.Figure().Size())
    return double(used_vertices.Size()) - double(used_vertices.SetSize());
  unsigned matched_vertices = 0;
  for (auto ph : problem.Hole().v) {
    for (auto ps : solution) {
      if (ps == ph) {
        matched_vertices += 1;
        break;
      }
    }
  }
  return matched_vertices;
}

void MCTPVertices::Run() {
  ResetSearch();
  ++nruns;
  if (best_score >= 0) {
    // Push search to find solution with one more vertex
    unsigned ibs = unsigned(best_score) + (correct_last_run ? 1 : 0);
    covered_vertices.Clear();
    for (unsigned i = 0; i < ibs; ++i) {
      unsigned ntotal = 0;
      for (unsigned j = 0; j < covered_vertices.SetSize(); ++j) {
        if (covered_vertices.HasKey(j)) continue;
        auto pv = problem.Hole().v[j];
        for (unsigned u = 0; u < used_vertices.SetSize(); ++u) {
          if (used_vertices.HasKey(u)) continue;
          for (auto p : valid_candidates[u][valid_candidates_index[u]]) {
            if (p == pv) {
              auto index = cache.Index(p);
              ntotal += points_stats[u][index].n;
            }
          }
        }
      }
      if (ntotal == 0) break;
      double logn = log(ntotal);
      double best_stat_score = -1e100;
      unsigned best_u = used_vertices.SetSize();
      I2Point pnext;
      for (unsigned j = 0; j < covered_vertices.SetSize(); ++j) {
        if (covered_vertices.HasKey(j)) continue;
        auto pv = problem.Hole().v[j];
        for (unsigned u = 0; u < used_vertices.SetSize(); ++u) {
          if (used_vertices.HasKey(u)) continue;
          for (auto p : valid_candidates[u][valid_candidates_index[u]]) {
            if (p == pv) {
              auto index = cache.Index(p);
              double d = points_stats[u][index].Score(logn, exploration_mult);
              if (best_stat_score < d) {
                best_stat_score = d;
                best_u = u;
                pnext = p;
              }
            }
          }
        }
      }
      assert(best_u < used_vertices.SetSize());
      AddPointFDC(best_u, pnext);
      if (force_stop) break;
    }
  }
  if (!force_stop) TBase::RunI();
  double dscore = DScore();
  UpdateStat(dscore);
  if (dscore > best_score) {
    best_score = dscore;
    if (dscore >= 0) best_solution = Solution(problem.Id(), solution);
  }
  correct_last_run = (dscore >= 0);
}

Solution MCTPVertices::Solve(const Problem& p) {
  std::cout << "Running MCTPVertices::Solve for problem " << p.Id()
            << std::endl;
  Timer t;
  InitSearch(p);
  unsigned target = std::min(p.Hole().Size(), p.Figure().Size());
  for (; (best_score < target - 0.5) &&
         (t.GetSeconds() < max_time_in_seconds) &&
         (cache.SegmentsMapSize() < (1u << 23));)
    Run();
  std::cout << "\tBest score = " << best_score << "\tMax possible = " << target
            << std::endl;
  return (best_score >= 0) ? best_solution : Solution();
}
