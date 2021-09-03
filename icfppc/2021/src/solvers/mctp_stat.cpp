#include "solvers/mctp_stat.h"

#include <cmath>

using namespace src_solvers;

double MCTPStat::Score(double log_n_total, double exploration_mult) const {
  static const double c = sqrt(2.0);
  if (n == 0) return (c + 1) * exploration_mult;
  return s / n + c * exploration_mult * sqrt(log_n_total / n);
}
