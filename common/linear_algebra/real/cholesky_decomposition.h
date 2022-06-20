#pragma once

#include "common/base.h"
#include "common/linear_algebra/vector.h"

#include <cmath>

namespace la {
namespace real {
// For positive-definite matrix.
// Both L and LT are saved in the L matrix.
template <class TTMatrix>
class CholeskyDecomposition {
 public:
  using TMatrix = TTMatrix;
  using TValue = typename TMatrix::TValue;

 protected:
  double eps_build, eps_solve;

 protected:
  TMatrix l;

 public:
  explicit CholeskyDecomposition(double _eps_build = 1e-10,
                                 double _eps_solve = 1e-5)
      : eps_build(_eps_build), eps_solve(_eps_solve), l(0) {}

  bool Build(const TMatrix& m) {
    assert(m.Rows() == m.Columns());
    unsigned n = m.Rows();
    l = m;
    for (unsigned i = 0; i < n; ++i) {
      auto sum = l(i, i);
      for (unsigned k = 0; k < i; ++k) sum -= l(i, k) * l(i, k);
      if (sum < -eps_build) {
        return false;
      } else if (sum < eps_build) {
        l(i, i) = 0.0;
        for (unsigned j = i + 1; j < n; ++j) l(i, j) = l(j, i) = 0.0;
      } else {
        double lii = sqrt(sum), liii = 1.0 / lii;
        l(i, i) = lii;
        for (unsigned j = i + 1; j < n; ++j) {
          sum = l(j, i);
          for (unsigned k = 0; k < i; ++k) sum -= l(i, k) * l(j, k);
          l(i, j) = l(j, i) = sum * liii;
        }
      }
    }
    return true;
  }

  unsigned Size() const { return l.Rows(); }

  double Det() const {
    double det = 1.0;
    for (unsigned i = 0; i < Size(); ++i) det *= l(i, i);
    return det * det;
  }

  // Solve Lx = b
  template <class TVector>
  bool SolveL(const TVector& b, TVector& output_x) const {
    unsigned n = Size();
    assert(b.Size() == n);
    if (&output_x != &b) output_x = b;
    for (unsigned i = 0; i < n; ++i) {
      double sum = b(i), lii = l(i, i);
      for (unsigned k = 0; k < i; ++k) sum -= l(i, k) * output_x(k);
      if (lii == 0.0) {
        if (fabs(sum) > eps_solve) return false;
        output_x(i) = 0.0;
      } else {
        output_x(i) = sum / lii;
      }
    }
    return true;
  }

  // Solve LTx = b
  template <class TVector>
  bool SolveLT(const TVector& b, TVector& output_x) const {
    unsigned n = Size();
    assert(b.Size() == n);
    if (&output_x != &b) output_x = b;
    for (unsigned i = n; i--;) {
      double sum = b(i), lii = l(i, i);
      for (unsigned k = i + 1; k < n; ++k) sum -= l(i, k) * output_x(k);
      if (lii == 0.0) {
        if (fabs(sum) > eps_solve) return false;
        output_x(i) = 0.0;
      } else {
        output_x(i) = sum / lii;
      }
    }
    return true;
  }

  // Solve Ax = b
  template <class TVector>
  bool Solve(const TVector& b, TVector& output_x) const {
    return SolveL(b, output_x) && SolveLT(output_x, output_x);
  }
};
}  // namespace real
}  // namespace la
