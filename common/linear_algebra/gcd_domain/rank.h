#pragma once

#include "common/base.h"
#include "common/linear_algebra/rows/minus.h"
#include "common/linear_algebra/rows/sub_m.h"
#include "common/linear_algebra/rows/swap.h"

namespace la {
namespace gcd {
template <class TMatrix>
constexpr unsigned Rank(const TMatrix& matrix) {
  using TValue = typename TMatrix::TValue;
  TMatrix m(matrix);
  unsigned r = 0;
  for (unsigned j = 0; (r < m.Rows()) && (j < m.Columns()); ++j) {
    unsigned best_row = r, non_zero = 0;
    TValue best_value = 0;
    for (unsigned i = r; i < m.Rows(); ++i) {
      if (m(i, j) < 0) rows::Minus(m, r, j);
      if (m(i, j) != 0) {
        ++non_zero;
        if ((best_value == 0) || (m(i, j) < best_value)) {
          best_value = m(i, j);
          best_row = i;
        }
      }
    }
    if (non_zero == 0) continue;
    rows::Swap(m, r, best_row, j);
    for (;;) {
      best_row = r;
      best_value = m(r, j);
      for (unsigned i = r + 1; i < m.Rows(); ++i) {
        if (m(i, j) == 0) continue;
        TValue rm = m(r, j) / m(i, j);
        rows::SubM(m, i, r, rm, j);
        if ((m(i, j) != 0) && (m(i, j) < best_value)) {
          best_value = m(i, j);
          best_row = i;
        }
      }
      if (best_row != r)
        rows::Swap(m, r, best_row, j);
      else
        break;
    }
    ++r;
  }
  return r;
}
}  // namespace gcd
}  // namespace la
