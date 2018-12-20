#pragma once

#include "matrix.h"

inline void MatrixRowsBAdd(MatrixBool& matrix, unsigned row_i, unsigned row_j,
                           unsigned row_first_block = 0) {
  MatrixBool::biterator it_i = matrix.GetBP(row_i, row_first_block),
                        it_j = matrix.GetBP(row_j, row_first_block),
                        it_i_end = matrix.GetBP(row_i + 1, 0);
  for (; it_i < it_i_end;) *it_i++ ^= *it_j++;
}
