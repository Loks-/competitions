#pragma once

#include "common/linear_algebra/matrix_static_size.h"

namespace geometry {
template <class TValue, unsigned _dim>
class AffineTransformation {
 public:
  static const unsigned dim = _dim;
  using T = TValue;
  using TSelf = AffineTransformation<T, dim>;

 protected:
  la::MatrixStaticSize<T, dim, dim + 1> m;

 public:
  AffineTransformation() { m.SetDiagonal(T(1)); }

  T& Get(unsigned i, unsigned j) {
    assert((i < dim) && (j <= dim));
    return m(i, j);
  }

  T Get(unsigned i, unsigned j) const {
    assert((i <= dim) && (j <= dim));
    return (i == dim) ? T((j == dim) ? 1 : 0) : m(i, j);
  }

  template <class TPoint>
  TPoint Apply(const TPoint& p) {
    static_assert(p.dim == dim);
    TPoint r;
    for (unsigned i = 0; i < dim; ++i) {
      r[i] = m(i, dim);
      for (unsigned j = 0; j < dim; ++j) r[i] += m(i, j) * p[j];
    }
    return r;
  }

  TSelf Apply(const TSelf& at) {
    TSelf r;
    for (unsigned i = 0; i < dim; ++i) {
      for (unsigned j = 0; j <= dim; ++j) {
        r.m(i, j) = 0;
        for (unsigned k = 0; k < dim; ++k) r.m(i, j) += m(i, k) * at.m(k, j);
      }
      r.m(i, dim) += m(i, dim);
    }
    return r;
  }

  template <class TPoint>
  TPoint operator()(const TPoint& p) {
    return Apply(p);
  }

  TSelf operator*(const TSelf& r) { return Apply(r); }
};
}  // namespace geometry
