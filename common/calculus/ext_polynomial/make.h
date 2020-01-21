#pragma once

#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"

namespace calculus {
namespace ext_polynomial {
// x
template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> MakeX() {
  auto t = Term<TValueF, TValueTerm>(TValueF(1));
  t.tp.power = 1;
  return t;
}

// x - c
template <class TValueF, class TValueTerm>
inline Function<TValueF, TValueTerm> MakeLinear(const TValueF& c) {
  return MakeX<TValueF, TValueTerm>() - c;
}

inline DFunction DMakeX() { return MakeX<double, double>(); }
inline DFunction DMakeLinear(double c) { return MakeLinear<double, double>(c); }
}  // namespace ext_polynomial
}  // namespace calculus
