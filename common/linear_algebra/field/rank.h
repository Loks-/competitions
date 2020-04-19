#pragma once

#include "common/linear_algebra/rows/sub_m.h"
#include "common/linear_algebra/rows/swap.h"

namespace la {
namespace field {
template <class TMatrix>
inline unsigned Rank(const TMatrix& matrix) {
  using TValue = typename TMatrix::TValue;
  TMatrix m(matrix);
  unsigned r = 0;
  for (unsigned j = 0; (r < m.Rows()) && (j < m.Columns()); ++j) {
    for (unsigned i = r; i < m.Rows(); ++i) {
      if (m(i, j)) {
        rows::Swap(m, r, i, j);
        break;
      }
    }
    if (!m(r, j)) continue;
    for (unsigned i = r + 1; i < m.Rows(); ++i) {
      if (!m(i, j)) continue;
      TValue rm = m(r, j) / m(i, j);
      rows::SubM(m, i, r, rm, j);
    }
    ++r;
  }
  return r;
}
}  // namespace field
}  // namespace la
