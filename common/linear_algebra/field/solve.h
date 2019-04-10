#pragma once

#include "common/base.h"
#include "common/linear_algebra/rows/sub_m.h"
#include "common/linear_algebra/rows/swap.h"
#include <algorithm>
#include <vector>

// Solve Ax = b
template <class TMatrix, class TVector>
inline bool FMatrixSolve(const TMatrix& A, const TVector& b,
                         TVector& output_x) {
  using TValue = typename TMatrix::TValue;
  assert((A.Rows() == b.Size()) && (A.Columns() == output_x.Size()));
  TMatrix m(A);
  TVector v(b);
  unsigned r = 0;
  std::vector<unsigned> vj(m.Rows());
  for (unsigned j = 0; (r < m.Rows()) && (j < m.Columns()); ++j) {
    for (unsigned i = r; i < m.Rows(); ++i) {
      if (m(i, j) != 0) {
        if (i != r) {
          MatrixRowsSwap(m, r, i, j);
          std::swap(v(r), v(i));
        }
        break;
      }
    }
    if (m(r, j) == 0) continue;
    for (unsigned i = r + 1; i < m.Rows(); ++i) {
      if (m(i, j) == 0) continue;
      TValue rm = m(r, j) / m(i, j);
      MatrixRowsSubM(m, i, r, rm, j);
      v(i) -= rm * v(r);
    }
    vj[r++] = j;
  }
  for (unsigned i = r; i < m.Rows(); ++i) {
    if (v(i) != 0) return false;
  }
  output_x.Fill(TValue(0));
  for (; r--;) {
    unsigned j = vj[r];
    if (v(r) == 0) continue;
    TValue rm = v(r) / m(r, j);
    for (unsigned i = 0; i < r; ++i) v(i) -= rm * m(i, j);
    output_x(j) = rm;
  }
  return true;
}
