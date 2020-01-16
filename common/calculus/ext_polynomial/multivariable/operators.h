#pragma once

#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/term_power.h"

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue, unsigned dim1, unsigned dim2>
inline Function<TValue, dim1 + dim2> CartesianProduct(
    const Function<TValue, dim1>& f1, const Function<TValue, dim2>& f2) {
  Function<TValue, dim1 + dim2> f;
  TermPower<TValue, dim1 + dim2> tp;
  for (auto& t1 : f1.v) {
    for (unsigned i = 0; i < dim1; ++i) tp(i) = t1.tp(i);
    for (auto& t2 : f2.v) {
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
  for (; (i < f1.v.size()) && (j < f2.v.size());) {
    auto& t1 = f1.v[i];
    auto& t2 = f2.v[j];
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
  for (; i < f1.v.size(); ++i) f.AddTerm(f1.v[i]);
  for (; j < f2.v.size(); ++j) f.AddTerm(f2.v[j]);
  if (compress_required) f.CompressSorted();
  return f;
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
inline Function<TValue, dim> operator-(const Function<TValue, dim>& f1,
                                       const Function<TValue, dim>& f2) {
  Function<TValue, dim> f;
  size_t i = 0, j = 0;
  bool compress_required = false;
  for (; (i < f1.v.size()) && (j < f2.v.size());) {
    auto& t1 = f1.v[i];
    auto& t2 = f2.v[j];
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
  for (; i < f1.v.size(); ++i) f.AddTerm(f1.v[i]);
  for (; j < f2.v.size(); ++j) f.AddTerm(-f2.v[j]);
  if (compress_required) f.CompressSorted();
  return f;
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
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
