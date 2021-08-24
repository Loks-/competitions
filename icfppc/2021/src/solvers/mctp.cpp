#include "solvers/mctp.h"

#include "evaluator.h"

#include "common/timer.h"

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace src_solvers;

double MCTP::Stat::Score(double log_n_total, double max_score) const {
  static const double c = sqrt(2.0);
  if (n == 0) return (c + 1) * max_score;
  return s / n + c * max_score * sqrt(log_n_total / n);
}

void MCTP::InitSearch(const Problem& p) {
  TBase::InitSearch(p);
  nruns = 0;
  best_score = 1e-10;
  location_stats.clear();
  location_stats.resize(cache.MaxIndex());
  points_stats.clear();
  points_stats.resize(problem.Figure().Size(), location_stats);
}

void MCTP::ResetSearch() { TBase::ResetSearch(); }

void MCTP::UpdateStat(double score) {
  for (unsigned u = 0; u < used_vertices.SetSize(); ++u) {
    if (used_vertices.HasKey(u)) {
      auto index = cache.Index(solution[u]);
      location_stats[index].Add(score);
      points_stats[u][index].Add(score);
    }
  }
}

void MCTP::Run() {
  ResetSearch();
  double logn = log(double(++nruns));
  unsigned gsize = problem.Figure().Size();
  for (; used_vertices.Size() < gsize;) {
    unsigned best_u = gsize;
    I2Point pnext;
    double best_stat_score = 0.;
    if (used_vertices.Size() == 0) {
      for (auto p : cache.GetValidPoints()) {
        auto index = cache.Index(p);
        double d1 = location_stats[index].Score(logn, best_score);
        for (unsigned u = 0; u < gsize; ++u) {
          double d2 = points_stats[u][index].Score(logn, best_score);
          if (best_stat_score < d1 + d2) {
            best_stat_score = d1 + d2;
            best_u = u;
            pnext = p;
          }
        }
      }
    } else {
      unsigned min_size = cache.MaxIndex() + 1;
      for (unsigned u = 0; u < gsize; ++u) {
        if (used_vertices.HasKey(u)) continue;
        if (valid_candidates[u][valid_candidates_index[u]].size() < min_size) {
          min_size = valid_candidates[u][valid_candidates_index[u]].size();
          best_u = u;
        }
      }
      if (min_size == 0) break;
      for (auto p : valid_candidates[best_u][valid_candidates_index[best_u]]) {
        auto index = cache.Index(p);
        double d = points_stats[best_u][index].Score(logn, best_score);
        if (best_stat_score < d) {
          best_stat_score = d;
          pnext = p;
        }
      }
    }
    AddPoint(best_u, pnext);
    if (force_stop) break;
  }
  double dscore = 0.;
  if (used_vertices.Size() == gsize) {
    Solution s{problem.Id(), solution};
    auto r = Evaluator::Apply(problem, s);
    if (r.DScore() > best_score) {
      //   std::cout << "New best solution: " << r.score << "\t" << nruns
      //             << std::endl;
      best_score = r.DScore();
      best_solution = s;
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
