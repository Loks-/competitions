#pragma once

#include "common/base.h"
#include "common/linear_algebra/rows/sub_m.h"
#include "common/linear_algebra/rows/swap.h"

#include <vector>

namespace la {
namespace field {
template <class TTMatrix>
class LUPDecomposition {
 public:
  using TMatrix = TTMatrix;
  using TValue = typename TMatrix::TValue;

 protected:
  TMatrix lu;
  std::vector<unsigned> p;
  TValue det_sign;

 public:
  LUPDecomposition() : lu(0), det_sign(1) {}

  bool Build(const TMatrix& m) {
    assert(m.Rows() == m.Columns());
    unsigned n = m.Rows();
    lu = m;
    p.resize(n);
    det_sign = 1;
    for (unsigned k = 0; k < n; ++k) {
      p[k] = k;
      for (unsigned i = k; i < n; ++i) {
        if (lu(i, k)) {
          if (i != k) {
            rows::Swap(lu, k, i);
            det_sign = -det_sign;
            p[k] = i;
          }
          break;
        }
      }
      if (!lu(k, k)) {
        for (unsigned i = k; i < n; ++i) {
          if (lu(i, k)) return false;
        }
      } else {
        TValue ilukk = TValue(1) / lu(k, k);
        for (unsigned i = k + 1; i < n; ++i) {
          lu(i, k) *= ilukk;
          rows::SubM(lu, i, k, lu(i, k), k + 1);
        }
      }
    }
    return true;
  }

  unsigned Size() const { return lu.Rows(); }

  TValue Det() const {
    TValue det = det_sign;
    for (unsigned i = 0; i < lu.Rows(); ++i) det *= lu(i, i);
    return det;
  }

  // Solve Ax = b
  template <class TVector>
  bool Solve(const TVector& b, TVector& output_x) const {
    unsigned n = lu.Rows();
    assert(b.Size() == n);
    output_x = b;
    unsigned ii = 0;
    for (unsigned i = 0; i < n; ++i) {
      unsigned ip = p[i];
      TValue sum = output_x(ip);
      output_x(ip) = output_x(i);
      if (ii != 0) {
        for (unsigned j = ii - 1; j < i; ++j) sum -= lu(i, j) * output_x(j);
      } else if (sum) {
        ii = i + 1;
      }
      output_x(i) = sum;
    }
    for (unsigned i = n; i--;) {
      TValue sum = output_x(i);
      for (unsigned j = i + 1; j < n; ++j) sum -= lu(i, j) * output_x(j);
      if (!lu(i, i)) {
        if (sum) return false;
        output_x(i) = 0;
      } else {
        output_x(i) = sum / lu(i, i);
      }
    }
    return true;
  }
};
}  // namespace field
}  // namespace la
