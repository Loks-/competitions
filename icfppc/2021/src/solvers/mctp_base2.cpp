#include "solvers/mctp_base2.h"

using namespace src_solvers;

void MCTPBase2::InitSearch(const Problem& p) {
  TBase::InitSearch(p);
  nruns = 0;
  best_score = 0.;
  exploration_mult = 1.;
  location_stats.clear();
  location_stats.resize(cache.MaxIndex());
  points_stats.clear();
  points_stats.resize(problem.Figure().Size(), location_stats);
}

void MCTPBase2::ResetSearch() { TBase::ResetSearch(); }

void MCTPBase2::UpdateStat(double score) {
  for (unsigned u : used_vertices) {
    auto index = cache.Index(solution[u]);
    location_stats[index].Add(score);
    points_stats[u][index].Add(score);
  }
}

void MCTPBase2::RunI() {
  unsigned gsize = problem.Figure().Size();
  for (; !unused_vertices.Empty();) {
    unsigned best_u = gsize;
    I2Point pnext;
    double best_stat_score = -1e100;
    if (use_location_stats && (used_vertices.size() == 0)) {
      unsigned ntotal = 0;
      for (auto p : cache.GetValidPoints()) {
        auto index = cache.Index(p);
        ntotal += location_stats[index].n;
      }
      double logn = log(ntotal);
      for (auto p : cache.GetValidPoints()) {
        auto index = cache.Index(p);
        double d1 = location_stats[index].Score(logn, exploration_mult);
        for (unsigned u = 0; u < gsize; ++u) {
          double d2 = points_stats[u][index].Score(logn, exploration_mult);
          if (best_stat_score < d1 + d2) {
            best_stat_score = d1 + d2;
            best_u = u;
            pnext = p;
          }
        }
      }
    } else {
      unsigned min_size = cache.MaxIndex() + 1;
      for (unsigned u : unused_vertices.List()) {
        if (remaining_order[u] == 0) continue;
        auto csize = valid_candidates[u][valid_candidates_index[u]].size();
        if (csize < min_size) {
          min_size = csize;
          best_u = u;
        }
      }
      if ((best_u == gsize) || (min_size == 0)) break;
      unsigned ntotal = 0;
      for (auto p : valid_candidates[best_u][valid_candidates_index[best_u]]) {
        auto index = cache.Index(p);
        ntotal += points_stats[best_u][index].n;
      }
      double logn = log(ntotal);
      for (auto p : valid_candidates[best_u][valid_candidates_index[best_u]]) {
        auto index = cache.Index(p);
        double d = points_stats[best_u][index].Score(logn, exploration_mult);
        if (best_stat_score < d) {
          best_stat_score = d;
          pnext = p;
        }
      }
    }
    AddPoint(best_u, pnext);
    if (force_stop) break;
  }
  if (!force_stop) {
    ZeroOrderOptimization();
    for (unsigned u : unused_vertices.List()) used_vertices.push_back(u);
    unused_vertices.Clear();
  }
}

void MCTPBase2::Run() {
  ResetSearch();
  ++nruns;
  RunI();
}
