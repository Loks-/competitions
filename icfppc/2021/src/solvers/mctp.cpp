#include "solvers/mctp.h"

#include "evaluator.h"

#include "common/base.h"
#include "common/geometry/d2/distance/distance_l2.h"
#include "common/geometry/d2/segment.h"

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
  problem = p;
  cache.Init(problem);
  nruns = 0;
  best_score = 1e-10;
  unsigned size = problem.Figure().Size();
  used_vertices.Resize(size);
  valid_candidates.clear();
  valid_candidates.resize(size);
  for (unsigned i = 0; i < size; ++i) {
    valid_candidates[i].resize(2 * size + 1);  // For safety
    valid_candidates[i][0] = cache.GetValidPoints();
  }
  valid_candidates_index.clear();
  valid_candidates_index.resize(size, 0);
  solution.clear();
  solution.resize(size);
  force_stop = false;
  location_stats.clear();
  location_stats.resize(cache.MaxIndex());
  points_stats.clear();
  points_stats.resize(size, location_stats);
}

void MCTP::ResetSearch() {
  used_vertices.Clear();
  std::fill(valid_candidates_index.begin(), valid_candidates_index.end(), 0);
  force_stop = false;
}

void MCTP::UpdateStat(double score) {
  for (unsigned u = 0; u < used_vertices.SetSize(); ++u) {
    if (used_vertices.HasKey(u)) {
      auto index = cache.Index(solution[u]);
      location_stats[index].Add(score);
      points_stats[u][index].Add(score);
    }
  }
}

void MCTP::AddPoint(unsigned index, const I2Point& p) {
  assert(!used_vertices.HasKey(index));
  used_vertices.Insert(index);
  solution[index] = p;
  for (auto e : problem.Figure().EdgesEI(index)) {
    unsigned u = e.to;
    if (used_vertices.HasKey(u)) {
      // Verify only
      auto p1 = solution[u];
      auto d = SquaredDistanceL2(p, p1);
      if ((d < e.info.first) || (d > e.info.second) ||
          !cache.Test(I2ClosedSegment(p, p1))) {
        std::cout << "SUS!" << std::endl;
        assert(false);
      }
    } else {
      // Filter points
      auto& vcurrent = valid_candidates[u][valid_candidates_index[u]];
      auto& vnext = valid_candidates[u][++valid_candidates_index[u]];
      vnext.clear();
      for (auto p1 : vcurrent) {
        auto d = SquaredDistanceL2(p, p1);
        if ((d >= e.info.first) && (d <= e.info.second) &&
            cache.TestI(I2ClosedSegment(p, p1))) {
          vnext.push_back(p1);
        }
      }
      if (vnext.empty()) force_stop = true;
    }
  }
}

void MCTP::RemoveLastPoint() {
  unsigned index = used_vertices.Last();
  for (auto e : problem.Figure().EdgesEI(index)) {
    unsigned u = e.to;
    if (!used_vertices.HasKey(u)) {
      --valid_candidates_index[u];
    }
  }
  used_vertices.RemoveLast();
  force_stop = false;
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
    Solution s{solution};
    auto r = Evaluator::Apply(problem, s);
    dscore = Evaluator::DScore(r);
    if (dscore > best_score) {
      std::cout << "New best solution: " << r.score << "\t" << nruns
                << std::endl;
      best_score = dscore;
      best_solution = s;
    }
  }
  UpdateStat(dscore);
}

Solution MCTP::Solve(const Problem& p) {
  Timer t;
  InitSearch(p);
  for (; (best_score < 1) && (t.GetSeconds() < max_time_in_seconds);) Run();
  return best_solution;
}
