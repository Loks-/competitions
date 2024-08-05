#pragma once

#include "common/base.h"

namespace numeric {
template <class TTValue>
class Complex {
 public:
  using TValue = TTValue;
  using TSelf = Complex<TTValue>;

 public:
  TValue re, im;

  constexpr Complex() : re(0), im(0) {}
  constexpr explicit Complex(const TValue& _re) : re(_re), im(0) {}
  constexpr Complex(const TValue& _re, const TValue& _im) : re(_re), im(_im) {}

  constexpr bool operator==(const TValue& r) const {
    return (re == r) && (im == TValue(0));
  }

  constexpr bool operator!=(const TValue& r) const {
    return (re != r) || (im != TValue(0));
  }

  constexpr bool operator==(const TSelf& r) const {
    return (re == r.re) && (im == r.im);
  }
  constexpr bool operator!=(const TSelf& r) const {
    return (re != r.re) || (im != r.im);
  }

  constexpr TSelf operator*(const TValue& r) const {
    return TSelf(re * r, im * r);
  }
  constexpr TSelf operator/(const TValue& r) const {
    return TSelf(re / r, im / r);
  }

  constexpr TSelf& operator+=(const TSelf& r) {
    re += r.re;
    im += r.im;
    return *this;
  }

  constexpr TSelf& operator-=(const TSelf& r) {
    re -= r.re;
    im -= r.im;
    return *this;
  }

  constexpr TSelf& operator*=(const TSelf& r) { return (*this = (*this * r)); }
  constexpr TSelf& operator/=(const TSelf& r) { return (*this = (*this / r)); }

  constexpr TSelf operator-() const { return TSelf(-re, -im); }
  constexpr TSelf operator+(const TSelf& r) const {
    return TSelf(re + r.re, im + r.im);
  }
  constexpr TSelf operator-(const TSelf& r) const {
    return TSelf(re - r.re, im - r.im);
  }

  constexpr TSelf operator*(const TSelf& r) const {
    return TSelf(re * r.re - im * r.im, re * r.im + im * r.re);
  }

  constexpr TSelf operator/(const TSelf& r) const {
    TValue tre = re * r.re + im * r.im, tim = im * r.re - re * r.im,
           td = r.re * r.re + r.im * r.im;
    assert(td != 0);
    return TSelf(tre / td, tim / td);
  }

  constexpr TSelf Conjugate() const { return TSelf(re, -im); }
  constexpr TValue AbsSquared() const { return re * re + im * im; }

  constexpr TSelf Inverse() const {
    TValue t = AbsSquared();
    return TSelf(re / t, -im / t);
  }

  constexpr TSelf PowU(uint64_t pow) const {
    TSelf ans = TSelf(1, 0), x = *this;
    for (; pow; pow >>= 1) {
      if (pow & 1) ans *= x;
      x *= x;
    }
    return ans;
  }

  constexpr TSelf PowS(int64_t pow) const {
    if (pow < 0)
      return Inverse().PowU(-pow);
    else
      return PowU(pow);
  }
};

template <class TValue>
constexpr Complex<TValue> operator*(const TValue& l, const Complex<TValue>& r) {
  return Complex<TValue>(r.re * l, r.im * l);
}

template <class TValue>
constexpr Complex<TValue> operator/(const TValue& l, const Complex<TValue>& r) {
  return Complex<TValue>(l) / r;
}
}  // namespace numeric
