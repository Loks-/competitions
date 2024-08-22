#pragma once

#include "common/linear_algebra/vector_static_size.h"

#include <algorithm>

namespace la {
template <class TTValue, unsigned rows, unsigned columns>
class MatrixStaticSize : public VectorStaticSize<TTValue, rows * columns> {
 public:
  using TValue = TTValue;
  using TSelf = MatrixStaticSize<TValue, rows, columns>;
  using TBase = VectorStaticSize<TValue, rows * columns>;

  using iterator = typename TBase::iterator;
  using const_iterator = typename TBase::const_iterator;

 public:
  static consteval unsigned Rows() { return rows; }
  static consteval unsigned Columns() { return columns; }
  static consteval bool IsSquare() { return (rows == columns); }

 public:
  constexpr MatrixStaticSize() {}

  // explicit MatrixStaticSize(const TValue& v) : TBase(v) {}

  constexpr explicit MatrixStaticSize(std::initializer_list<TValue> l)
      : TBase(l) {}

  constexpr TSelf& operator=(const TValue& v) {
    TBase::Fill(v);
    return *this;
  }

  constexpr TValue& operator()(unsigned i, unsigned j) {
    return TBase::data[i * columns + j];
  }

  constexpr const TValue& operator()(unsigned i, unsigned j) const {
    return TBase::data[i * columns + j];
  }

  constexpr iterator GetP(unsigned i, unsigned j) {
    return TBase::GetP(i * columns + j);
  }

  constexpr const_iterator GetP(unsigned i, unsigned j) const {
    return TBase::GetP(i * columns + j);
  }

  constexpr void swap(TSelf& r) { TBase::swap(r); }

  constexpr void SetDiagonal(const TValue& v) {
    unsigned diagonal_length = std::min(rows, columns);
    const unsigned shift = columns + 1;
    for (iterator p = TBase::begin(); diagonal_length--; p += shift) *p = v;
  }

  constexpr TSelf& operator+=(const TSelf& v) {
    TBase::operator+=(v);
    return *this;
  }

  constexpr TSelf& operator-=(const TSelf& v) {
    TBase::operator-=(v);
    return *this;
  }

  constexpr TSelf operator+(const TSelf& v) const {
    TSelf t(*this);
    t += v;
    return t;
  }

  constexpr TSelf operator-(const TSelf& v) const {
    TSelf t(*this);
    t -= v;
    return t;
  }

  template <unsigned columns2>
  constexpr void Mult(const MatrixStaticSize<TValue, columns, columns2>& v,
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
  constexpr MatrixStaticSize<TValue, rows, columns2> operator*(
      const MatrixStaticSize<TValue, columns, columns2>& v) const {
    MatrixStaticSize<TValue, rows, columns2> t;
    Mult(v, t);
    return t;
  }

  // Operations with square matrix
  constexpr TSelf& operator*=(const TSelf& v) {
    static_assert(IsSquare(), "matrix should be square");
    TSelf t;
    Mult(v, t);
    swap(t);
    return *this;
  }

  constexpr TSelf PowU(uint64_t pow) const {
    static_assert(IsSquare(), "matrix should be square");
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
