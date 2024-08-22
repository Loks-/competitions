#pragma once

#include "common/base.h"
#include "common/linear_algebra/rows/sub_m.h"
#include "common/linear_algebra/rows/swap.h"

#include <cmath>

namespace la {
namespace real {
template <class TMatrix>
constexpr double Det(const TMatrix& matrix, double eps = 1e-10) {
  double det = 1.0;
  TMatrix m(matrix);
  assert(m.Rows() == m.Columns());
  const unsigned n = m.Rows();
  for (unsigned k = 0; k < n; ++k) {
    double max_value = fabs(m(k, k));
    unsigned imax = k;
    for (unsigned i = k + 1; i < n; ++i) {
      double temp = fabs(m(i, k));
      if (temp > max_value) {
        max_value = temp;
        imax = i;
      }
    }
    if (max_value < eps) return 0.;
    if (imax != k) {
      rows::Swap(m, k, imax);
      det = -det;
    }
    det *= m(k, k);
    double km = 1.0 / m(k, k);
    for (unsigned i = k + 1; i < n; ++i)
      rows::SubM(m, i, k, m(i, k) * km, k + 1);
  }
  return det;
}
}  // namespace real
}  // namespace la
