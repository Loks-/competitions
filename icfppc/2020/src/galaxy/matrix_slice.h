#pragma once

template <class TMatrix>
class MatrixSlice {
 protected:
  const TMatrix& m;
  unsigned rb, re, cb, ce;

 public:
  using TValue = typename TMatrix::TValue;

  MatrixSlice(const TMatrix& _m)
      : m(_m), rb(0), re(m.Rows()), cb(0), ce(m.Columns()) {}

  MatrixSlice(const TMatrix& _m, unsigned _rb, unsigned _re)
      : m(_m), rb(_rb), re(_re), cb(0), ce(m.Columns()) {}

  MatrixSlice(const TMatrix& _m, unsigned _rb, unsigned _re, unsigned _cb,
              unsigned _ce)
      : m(_m), rb(_rb), re(_re), cb(_cb), ce(_ce) {}

  MatrixSlice(const MatrixSlice& ms)
      : m(ms.m), rb(ms.rb), re(ms.re), cb(ms.cb), ce(ms.ce) {}

  MatrixSlice(const MatrixSlice& ms, unsigned _rb, unsigned _re)
      : m(ms.m), rb(ms.rb + _rb), re(ms.rb + _re), cb(ms.cb), ce(ms.ce) {}

  MatrixSlice(const MatrixSlice& ms, unsigned _rb, unsigned _re, unsigned _cb,
              unsigned _ce)
      : m(ms.m),
        rb(ms.rb + _rb),
        re(ms.rb + _re),
        cb(ms.cb + _cb),
        ce(ms.cb + _ce) {}

  unsigned Rows() const { return re - rb; }
  unsigned Columns() const { return ce - cb; }

  TValue Get(unsigned r, unsigned c) const { return m.Get(rb + r, cb + c); }

  bool IsRowEmpty(unsigned r) const {
    for (unsigned c = 0; c < Columns(); ++c) {
      if (Get(r, c)) return false;
    }
    return true;
  }

  bool IsColumnEmpty(unsigned c) const {
    for (unsigned r = 0; r < Rows(); ++r) {
      if (Get(r, c)) return false;
    }
    return true;
  }
};
