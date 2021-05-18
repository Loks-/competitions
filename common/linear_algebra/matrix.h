#pragma once

#include "common/linear_algebra/vector.h"
#include <algorithm>

namespace la {
template <class TTValue>
class Matrix : public Vector<TTValue> {
 public:
  using TValue = TTValue;
  using TSelf = Matrix<TValue>;
  using TBase = Vector<TValue>;

  using iterator = typename TBase::iterator;
  using const_iterator = typename TBase::const_iterator;

 protected:
  unsigned rows;
  unsigned columns;

 public:
  unsigned Rows() const { return rows; }
  unsigned Columns() const { return columns; }

  explicit Matrix(unsigned size)
      : TBase(size * size), rows(size), columns(size) {}

  Matrix(unsigned _rows, unsigned _columns)
      : TBase(_rows * _columns), rows(_rows), columns(_columns) {}

  Matrix(unsigned _rows, unsigned _columns, const TValue& v)
      : TBase(_rows * _columns, v), rows(_rows), columns(_columns) {}

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

  iterator begin() { return TBase::begin(); }
  const_iterator begin() const { return TBase::begin(); }
  iterator end() { return TBase::end(); }
  const_iterator end() const { return TBase::end(); }

  iterator GetP(unsigned i, unsigned j) { return TBase::GetP(i * columns + j); }

  const_iterator GetP(unsigned i, unsigned j) const {
    return TBase::GetP(i * columns + j);
  }

  void swap(TSelf& r) {
    TBase::swap(r);
    std::swap(rows, r.rows);
    std::swap(columns, r.columns);
  }

  void SetDiagonal(const TValue& v) {
    unsigned diagonal_length = std::min(rows, columns);
    unsigned shift = columns + 1;
    for (iterator p = begin(), pend = p + diagonal_length * shift; p < pend;
         p += shift)
      *p = v;
  }

  TSelf& operator*=(const TValue& v) {
    TBase::operator*=(v);
    return *this;
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

  void Mult(const TSelf& v, TSelf& output) const {
    assert((v.rows == columns) && (output.rows == rows) &&
           (output.columns == v.columns));
    unsigned columns2 = output.columns;
    output.Clear();
    const_iterator pA = begin();
    for (unsigned i = 0; i < rows; ++i) {
      for (const_iterator pB = v.begin(), pBend = v.end(); pB < pBend;) {
        const TValue& vA = *pA++;
        for (iterator pC = output.GetP(i, 0), pCend = pC + columns2;
             pC < pCend;)
          *pC++ += *pB++ * vA;
      }
    }
  }

  void MultAx(const TBase& x, TBase& output) const {
    assert((x.Size() == columns) && (output.Size() == rows));
    output.Clear();
    const_iterator pA = begin(), pBbegin = x.begin(), pBend = x.end();
    for (iterator pO = output.begin(), pOend = output.end(); pO < pOend; ++pO) {
      for (const_iterator pB = pBbegin; pB < pBend;) *pO += *pA++ * *pB++;
    }
  }

  TBase operator*(const TBase& x) const {
    TBase t(rows);
    MultAx(x, t);
    return t;
  }

  TSelf operator*(const TSelf& v) const {
    TSelf t(rows, v.columns);
    Mult(v, t);
    return t;
  }

  TSelf& operator*=(const TSelf& v) {
    TSelf t(rows, v.columns);
    Mult(v, t);
    swap(t);
    return *this;
  }

  TSelf PowU(uint64_t pow) const {
    assert(rows == columns);
    TSelf ans(rows, columns);
    ans.SetDiagonal(TValue(1));
    TSelf x = *this;
    for (; pow; pow >>= 1) {
      if (pow & 1) ans *= x;
      x *= x;
    }
    return ans;
  }

  void AddXXT(const TBase& x) {
    assert((x.Size() == rows) && (rows == columns));
    iterator p = begin();
    for (const_iterator pxr = x.begin(), px_end = x.end(); pxr < px_end;
         ++pxr) {
      for (const_iterator pxc = x.begin(); pxc < px_end;) *p++ += *pxr * *pxc++;
    }
  }
};
}  // namespace la

using DMatrix = la::Matrix<double>;
