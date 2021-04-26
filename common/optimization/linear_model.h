#pragma once

#include "common/base.h"
#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/real/lup_decomposition.h"

namespace opt {
template <class TMatrix, class TVector1, class TVector2>
inline void LinearModel(const TMatrix& x, const TVector1& y, const TVector1& w,
                        TVector2& output_beta) {
  using TValue = typename TVector::TValue;
  unsigned rows = x.Rows(), columns = x.Columns();
  assert((y.Size() == rows) && (w.Size() == rows) &&
         (output_beta.Size() == columns));
  la::Matrix<TValue> xtx(columns, columns);
}

template <class TMatrix, class TVector1, class TVector2>
inline void LinearModel(const TMatrix& x, const TVector1& y,
                        TVector2& output_beta) {
  TVector1 w(y.size(), 1);
  LinearModel(x, y, w, output_beta);
}
}  // namespace opt
