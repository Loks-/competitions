#pragma once

#include "common/linear_algebra/matrix_static_size.h"

namespace geometry {
template <class TValue, unsigned dim>
class AffineTransformation {
 public:
  using T = TValue;
  using TSelf = AffineTransformation<T, dim>;

 protected:
  la::MatrixStaticSize<T, dim, dim + 1> m;

 public:
  static consteval unsigned Dim() { return dim; }

  constexpr AffineTransformation() { m.SetDiagonal(T(1)); }

  constexpr T& Get(unsigned i, unsigned j) {
    assert((i < dim) && (j <= dim));
    return m(i, j);
  }

  constexpr T Get(unsigned i, unsigned j) const {
    assert((i <= dim) && (j <= dim));
    return (i == dim) ? T((j == dim) ? 1 : 0) : m(i, j);
  }

  template <class TPoint>
  constexpr TPoint Apply(const TPoint& p) const {
    static_assert(TPoint::Dim() == dim);
    TPoint r;
    for (unsigned i = 0; i < dim; ++i) {
      r[i] = m(i, dim);
      for (unsigned j = 0; j < dim; ++j) r[i] += m(i, j) * p[j];
    }
    return r;
  }

  constexpr TSelf Apply(const TSelf& at) const {
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
  constexpr TPoint operator()(const TPoint& p) const {
    return Apply(p);
  }

  constexpr TSelf operator*(const TSelf& r) const { return Apply(r); }
};
}  // namespace geometry
