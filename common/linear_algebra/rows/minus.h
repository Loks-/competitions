#pragma once

namespace la {
namespace rows {
template <class TMatrix>
inline void Minus(TMatrix& matrix, unsigned row,
                  unsigned row_first_element = 0) {
  typename TMatrix::iterator it = matrix.GetP(row, row_first_element),
                             it_end = matrix.GetP(row + 1, 0);
  for (; it < it_end; ++it) *it = -*it;
}
}  // namespace rows
}  // namespace la
