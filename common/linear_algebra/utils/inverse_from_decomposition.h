#pragma once

#include "common/linear_algebra/vector.h"

namespace la {
template <class TMatrix, class TDecomposition>
inline bool InverseFromDecomposition(const TDecomposition& md,
                                     TMatrix& output) {
  using TValue = typename TMatrix::TValue;
  unsigned size = md.Size();
  la::Vector<TValue> b(size, 0), x(size);
  TMatrix m(size);
  for (unsigned i = 0; i < size; ++i) {
    b(i) = TValue(1);
    if (!Solve(b, x)) return false;
    b(i) = TValue(0);
    for (unsigned j = 0; j < size; ++j) m(j, i) = x(j);
  }
  output.swap(m);
  return true;
}
}  // namespace la
