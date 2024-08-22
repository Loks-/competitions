#pragma once

#include "common/base.h"
#include "common/linear_algebra/rows/sub_m.h"
#include "common/linear_algebra/rows/swap.h"
#include "common/numeric/utils/abs.h"

#include <algorithm>
#include <vector>

namespace la {
namespace real {
template <class TTMatrix>
class LUPDecomposition {
 public:
  using TMatrix = TTMatrix;
  using TValue = typename TMatrix::TValue;

 protected:
  double eps_build, eps_solve;

 protected:
  TMatrix lu;
  std::vector<unsigned> p;
  double det_sign;

 public:
  constexpr explicit LUPDecomposition(double _eps_build = 1e-10,
                                      double _eps_solve = 1e-5)
      : eps_build(_eps_build), eps_solve(_eps_solve), lu(0), det_sign(1.0) {}

  constexpr bool Build(const TMatrix& m) {
    assert(m.Rows() == m.Columns());
    const unsigned n = m.Rows();
    lu = m;
    p.resize(n);
    std::vector<double> vv(n);
    det_sign = 1.0;
    for (unsigned i = 0; i < n; ++i) {
      double max_value = 0.0;
      for (unsigned j = 0; j < n; ++j)
        max_value = std::max(max_value, Abs(lu(i, j)));
      vv[i] = (max_value < eps_build) ? 1.0 : 1.0 / max_value;
    }
    for (unsigned k = 0; k < n; ++k) {
      double max_value = 0.0;
      unsigned imax = k;
      for (unsigned i = k; i < n; ++i) {
        double temp = vv[i] * Abs(lu(i, k));
        if (temp > max_value) {
          max_value = temp;
          imax = i;
        }
      }
      if (imax != k) {
        rows::Swap(lu, k, imax);
        det_sign = -det_sign;
        vv[imax] = vv[k];
      }
      p[k] = imax;
      if (Abs(lu(k, k)) <= eps_build) {
        for (unsigned i = k; i < n; ++i) {
          if (Abs(lu(i, k)) > eps_build) return false;
          lu(i, k) = 0.0;
        }
      } else {
        TValue ilukk = 1.0 / lu(k, k);
        for (unsigned i = k + 1; i < n; ++i) {
          lu(i, k) *= ilukk;
          rows::SubM(lu, i, k, lu(i, k), k + 1);
        }
      }
    }
    return true;
  }

  constexpr unsigned Size() const { return lu.Rows(); }

  constexpr TValue Det() const {
    TValue det = det_sign;
    for (unsigned i = 0; i < Size(); ++i) det *= lu(i, i);
    return det;
  }

  // Solve Ax = b
  template <class TVector>
  constexpr bool Solve(const TVector& b, TVector& output_x) const {
    const unsigned n = Size();
    assert(b.Size() == n);
    if (&output_x != &b) output_x = b;
    unsigned ii = 0;
    for (unsigned i = 0; i < n; ++i) {
      const unsigned ip = p[i];
      TValue sum = output_x(ip);
      output_x(ip) = output_x(i);
      if (ii != 0) {
        for (unsigned j = ii - 1; j < i; ++j) sum -= lu(i, j) * output_x(j);
      } else if (sum != 0.0) {
        ii = i + 1;
      }
      output_x(i) = sum;
    }
    for (unsigned i = n; i--;) {
      TValue sum = output_x(i);
      for (unsigned j = i + 1; j < n; ++j) sum -= lu(i, j) * output_x(j);
      if (lu(i, i) == 0.0) {
        if (Abs(sum) > eps_solve) return false;
        output_x(i) = 0.0;
      } else {
        output_x(i) = sum / lu(i, i);
      }
    }
    return true;
  }
};
}  // namespace real
}  // namespace la
