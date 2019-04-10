#pragma once

#include "common/linear_algebra/bool/matrix.h"
#include "common/linear_algebra/bool/rows_block_add.h"
#include "common/linear_algebra/bool/rows_block_swap.h"

inline unsigned BMatrixRank(const MatrixBool& matrix) {
  MatrixBool m(matrix);
  unsigned r = 0;
  for (unsigned j = 0; (r < m.Rows()) && (j < m.Columns()); ++j) {
    for (unsigned i = r; i < m.Rows(); ++i) {
      if (m.GetBit(i, j)) {
        MatrixRowsBSwap(m, r, i, j / MatrixBool::bits_per_block);
        break;
      }
    }
    if (m.GetBit(r, j) == 0) continue;
    for (unsigned i = r + 1; i < m.Rows(); ++i) {
      if (m.GetBit(i, j) == 0) continue;
      MatrixRowsBAdd(m, i, r, j / MatrixBool::bits_per_block);
    }
    ++r;
  }
  return r;
}
