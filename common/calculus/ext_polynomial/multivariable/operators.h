#pragma once

#include "common/assert_exception.h"
#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/operators/division.h"
#include "common/calculus/ext_polynomial/multivariable/operators/multiplication.h"
#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/ext_polynomial/multivariable/term_power.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue, unsigned dim1, unsigned dim2>
inline Function<TValue, dim1 + dim2> CartesianMultiplication(
    const Function<TValue, dim1>& f1, const Function<TValue, dim2>& f2) {
  Function<TValue, dim1 + dim2> f;
  TermPower<TValue, dim1 + dim2> tp;
  for (auto& t1 : f1.terms) {
    for (unsigned i = 0; i < dim1; ++i) tp(i) = t1.tp(i);
    for (auto& t2 : f2.terms) {
      for (unsigned i = 0; i < dim2; ++i) tp(i + dim1) = t2.tp(i);
      f.AddTerm({t1.a * t2.a, tp});
    }
  }
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator+(const Function<TValue, dim>& f1,
                                       const Function<TValue, dim>& f2) {
  Function<TValue, dim> f;
  size_t i = 0, j = 0;
  bool compress_required = false;
  for (; (i < f1.Size()) && (j < f2.Size());) {
    auto& t1 = f1(i);
    auto& t2 = f2(j);
    if (t1.tp < t2.tp) {
      f.AddTerm(t1);
      ++i;
    } else if (t2.tp < t1.tp) {
      f.AddTerm(t2);
      ++j;
    } else if (t1.tp == t2.tp) {
      TValue a = t1.a + t2.a;
      if (a != TValue(0)) f.AddTerm(Term<TValue, dim>(a, t1.tp));
      ++i;
      ++j;
    } else {
      f.AddTerm(t1);
      ++i;
      compress_required = true;
    }
  }
  for (; i < f1.Size(); ++i) f.AddTerm(f1(i));
  for (; j < f2.Size(); ++j) f.AddTerm(f2(j));
  if (compress_required) f.CompressSorted();
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator+=(Function<TValue, dim>& f1,
                                         const Function<TValue, dim>& f2) {
  return f1 = (f1 + f2);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator+(const Function<TValue, dim>& f1,
                                       const TValue& v2) {
  return f1 + Function<TValue, dim>(v2);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator+(const TValue& v1,
                                       const Function<TValue, dim>& f2) {
  return Function<TValue, dim>(v1) + f2;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator+=(Function<TValue, dim>& f1,
                                         const TValue& v2) {
  return f1 = (f1 + v2);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator-(const Function<TValue, dim>& f1,
                                       const Function<TValue, dim>& f2) {
  Function<TValue, dim> f;
  size_t i = 0, j = 0;
  bool compress_required = false;
  for (; (i < f1.Size()) && (j < f2.Size());) {
    auto& t1 = f1(i);
    auto& t2 = f2(j);
    if (t1.tp < t2.tp) {
      f.AddTerm(t1);
      ++i;
    } else if (t2.tp < t1.tp) {
      f.AddTerm(-t2);
      ++j;
    } else if (t1.tp == t2.tp) {
      TValue a = t1.a - t2.a;
      if (a != TValue(0)) f.AddTerm(Term<TValue, dim>(a, t1.tp));
      ++i;
      ++j;
    } else {
      f.AddTerm(t1);
      ++i;
      compress_required = true;
    }
  }
  for (; i < f1.Size(); ++i) f.AddTerm(f1(i));
  for (; j < f2.Size(); ++j) f.AddTerm(-f2(j));
  if (compress_required) f.CompressSorted();
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator-=(Function<TValue, dim>& f1,
                                         const Function<TValue, dim>& f2) {
  return f1 = (f1 - f2);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator-(const Function<TValue, dim>& f1,
                                       const TValue& v2) {
  return f1 - Function<TValue, dim>(v2);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator-(const TValue& v1,
                                       const Function<TValue, dim>& f2) {
  return Function<TValue, dim>(v1) - f2;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator-=(Function<TValue, dim>& f1,
                                         const TValue& v2) {
  return f1 = (f1 - v2);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator*=(Function<TValue, dim>& f1,
                                         const TValue& v2) {
  for (auto& t : f1.terms) t.a *= v2;
  return f1;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator*(const Function<TValue, dim>& f1,
                                       const TValue& v2) {
  Function<TValue, dim> f(f1);
  f *= v2;
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator*(const TValue& v1,
                                       const Function<TValue, dim>& f2) {
  Function<TValue, dim> f(f2);
  f *= v1;
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator*(const Function<TValue, dim>& f1,
                                       const Function<TValue, dim>& f2) {
  // Current version assume that all Multiplications are simple.
  Function<TValue, dim> f;
  for (auto& t1 : f1.terms) {
    for (auto& t2 : f2.terms) {
      f.AddTerm(operators::SimpleMultiplication(t1, t2));
    }
  }
  f.Compress();
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator*=(Function<TValue, dim>& f1,
                                         const Function<TValue, dim>& f2) {
  return f1 = (f1 * f2);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator*=(Function<TValue, dim>& f1,
                                         const TermPower<TValue, dim>& tp2) {
  if (tp2.IsTypeOne()) {
    for (auto& t1 : f1.terms)
      t1.tp = operators::SimpleMultiplication(t1.tp, tp2);
  } else {
    f1 *= Function<TValue, dim>(tp2);
  }
  return f1;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator*(const Function<TValue, dim>& f1,
                                       const TermPower<TValue, dim>& tp2) {
  Function<TValue, dim> f(f1);
  f *= tp2;
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator*=(Function<TValue, dim>& f1,
                                         const Term<TValue, dim>& t2) {
  if (t2.tp.IsTypeOne()) {
    for (auto& t1 : f1.terms) t1 = operators::SimpleMultiplication(t1, t2);
  } else {
    f1 *= Function<TValue, dim>(t2);
  }
  return f1;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator*(const Function<TValue, dim>& f1,
                                       const Term<TValue, dim>& t2) {
  Function<TValue, dim> f(f1);
  f *= t2;
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator/=(Function<TValue, dim>& f1,
                                         const TValue& v2) {
  for (auto& t : f1) t.a /= v2;
  return f1;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator/(const Function<TValue, dim>& f1,
                                       const TValue& v2) {
  Function<TValue, dim> f(f1);
  f /= v2;
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator/=(Function<TValue, dim>& f1,
                                         const TermPower<TValue, dim>& tp2) {
  Assert(tp2.IsTypeOne(), "Only simple division is supported.");
  for (auto& t1 : f1.terms) t1.tp = operators::SimpleDivision(t1.tp, tp2);
  return f1;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator/(const Function<TValue, dim>& f1,
                                       const TermPower<TValue, dim>& tp2) {
  Function<TValue, dim> f(f1);
  f /= tp2;
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator/=(Function<TValue, dim>& f1,
                                         const Term<TValue, dim>& t2) {
  Assert(t2.tp.IsTypeOne(), "Only simple division is supported.");
  for (auto& t1 : f1.terms) t1 = operators::SimpleDivision(t1, t2);
  return f1;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator/(const Function<TValue, dim>& f1,
                                       const Term<TValue, dim>& t2) {
  Function<TValue, dim> f(f1);
  f /= t2;
  return f;
}

template <class TValue, unsigned dim>
inline Function<TValue, dim>& operator/=(Function<TValue, dim>& f1,
                                         const Function<TValue, dim>& f2) {
  Assert(f2.Size() == 1, "Function should have exactly one term.");
  return f1 /= f2(0);
}

template <class TValue, unsigned dim>
inline Function<TValue, dim> operator/(const Function<TValue, dim>& f1,
                                       const Function<TValue, dim>& f2) {
  Function<TValue, dim> f(f1);
  f /= f2;
  return f;
}
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
