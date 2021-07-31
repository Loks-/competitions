#pragma once

#include "cache.h"
#include "problem.h"
#include "solution.h"
#include "solvers/base.h"

#include "common/data_structures/unsigned_set.h"
#include "common/geometry/d2/point.h"
#include "common/solvers/solver.h"
#include "common/timer.h"

#include <memory>
#include <string>
#include <vector>

namespace src_solvers {
class MCTP : public Base {
 public:
  using TBase = Base;

  class Stat {
   public:
    unsigned n = 0;
    double s = 0;

    void Add(double x) {
      ++n;
      s += x;
    }

    double Score(double log_n_total, double max_score) const;
  };

 protected:
  Problem problem;
  Cache cache;
  ds::UnsignedSet used_vertices;
  std::vector<std::vector<std::vector<I2Point>>> valid_candidates;
  std::vector<unsigned> valid_candidates_index;
  std::vector<I2Point> solution;
  bool force_stop;
  std::vector<std::vector<Stat>> points_stats;
  std::vector<Stat> location_stats;
  unsigned nruns;
  double best_score;
  Solution best_solution;

 protected:
  void InitSearch(const Problem& p);
  void ResetSearch();
  void UpdateStat(double score);
  void AddPoint(unsigned index, const I2Point& p);
  void RemoveLastPoint();
  void Run();

 public:
  MCTP(unsigned max_time_in_seconds) : TBase(max_time_in_seconds) {}

  PSolver Clone() const override { return std::make_shared<MCTP>(*this); }

  std::string Name() const override {
    return "mctp_" + std::to_string(max_time_in_seconds);
  }

  Solution Solve(const Problem& p) override;
};
}  // namespace src_solvers
