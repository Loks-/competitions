#pragma once

namespace src_solvers {
class MCTPStat {
 public:
  unsigned n = 0;
  double s = 0;

  void Add(double x) {
    ++n;
    s += x;
  }

  double Score(double log_n_total, double exploration_mult) const;
};
}  // namespace src_solvers
