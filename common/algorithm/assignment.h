#pragma once

#include "common/algorithm/assignment/hungarian.h"

namespace alg {
template <class TMatrix>
inline typename TMatrix::TValue Assignment(
    const TMatrix& a, const typename TMatrix::TValue& max_value) {
  assignment::Hungarian s;
  return s.Solve(a, max_value);
}
}  // namespace alg
