#pragma once

#include "common/linear_algebra/vector_static_size.h"

#include <algorithm>

namespace la {
template <class TTValue, unsigned _rows, unsigned _columns>
class MatrixStaticSize : public VectorStaticSize<TTValue, _rows * _columns> {
 public:
  static const unsigned rows = _rows;
  static const unsigned columns = _columns;
  static const bool is_square = (rows == columns);

  using TValue = TTValue;
  using TSelf = MatrixStaticSize<TValue, rows, columns>;
  using TBase = VectorStaticSize<TValue, rows * columns>;

  using iterator = typename TBase::iterator;
  using const_iterator = typename TBase::const_iterator;

 public:
  unsigned Rows() const { return rows; }
  unsigned Columns() const { return columns; }

  MatrixStaticSize() {}

  // explicit MatrixStaticSize(const TValue& v) : TBase(v) {}

  explicit MatrixStaticSize(std::initializer_list<TValue> l) : TBase(l) {}

  TSelf& operator=(const TValue& v) {
    TBase::Fill(v);
    return *this;
  }

  TValue& operator()(unsigned i, unsigned j) {
    return TBase::data[i * columns + j];
  }

  const TValue& operator()(unsigned i, unsigned j) const {
    return TBase::data[i * columns + j];
  }

  iterator GetP(unsigned i, unsigned j) { return TBase::GetP(i * columns + j); }

  const_iterator GetP(unsigned i, unsigned j) const {
    return TBase::GetP(i * columns + j);
  }

  void swap(TSelf& r) { TBase::swap(r); }

  void SetDiagonal(const TValue& v) {
    unsigned diagonal_length = std::min(rows, columns);
    unsigned shift = columns + 1;
    for (iterator p = TBase::begin(); diagonal_length--; p += shift) *p = v;
  }

  TSelf& operator+=(const TSelf& v) {
    TBase::operator+=(v);
    return *this;
  }

  TSelf& operator-=(const TSelf& v) {
    TBase::operator-=(v);
    return *this;
  }

  TSelf operator+(const TSelf& v) const {
    TSelf t(*this);
    t += v;
    return t;
  }

  TSelf operator-(const TSelf& v) const {
    TSelf t(*this);
    t -= v;
    return t;
  }

  template <unsigned columns2>
  void Mult(const MatrixStaticSize<TValue, columns, columns2>& v,
            MatrixStaticSize<TValue, rows, columns2>& output) const {
    output.Clear();
    const_iterator pA = TBase::begin();
    for (unsigned i = 0; i < rows; ++i) {
      for (const_iterator pB = v.begin(), pBend = v.end(); pB < pBend;) {
        const TValue& vA = *pA++;
        for (iterator pC = output.GetP(i, 0), pCend = pC + columns2;
             pC < pCend;)
          *pC++ += *pB++ * vA;
      }
    }
  }

  template <unsigned columns2>
  MatrixStaticSize<TValue, rows, columns2> operator*(
      const MatrixStaticSize<TValue, columns, columns2>& v) const {
    MatrixStaticSize<TValue, rows, columns2> t;
    Mult(v, t);
    return t;
  }

  // Operations with square matrix
  TSelf& operator*=(const TSelf& v) {
    static_assert(is_square, "matrix should be square");
    TSelf t;
    Mult(v, t);
    swap(t);
    return *this;
  }

  TSelf PowU(uint64_t pow) const {
    static_assert(is_square, "matrix should be square");
    TSelf ans;
    ans.SetDiagonal(TValue(1));
    TSelf x = *this;
    for (; pow; pow >>= 1) {
      if (pow & 1) ans *= x;
      x *= x;
    }
    return ans;
  }
};
}  // namespace la

template <unsigned rows, unsigned columns>
using DMatrixStaticSize = la::MatrixStaticSize<double, rows, columns>;
template <class TValue, unsigned size>
using SquareMatrixStaticSize = la::MatrixStaticSize<TValue, size, size>;
template <unsigned size>
using DSquareMatrixStaticSize = la::MatrixStaticSize<double, size, size>;
