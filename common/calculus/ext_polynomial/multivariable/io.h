#pragma once

#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/term.h"
#include "common/calculus/ext_polynomial/multivariable/term_power.h"
#include <iostream>

namespace calculus {
namespace ext_polynomial {
namespace multivariable {
template <class TValue, unsigned dim>
inline std::ostream& operator<<(std::ostream& s,
                                const TermPower<TValue, dim>& tp) {
  bool first = true;
  for (unsigned i = 0; i < dim; ++i) {
    std::string sname = tp(i).ToString("x" + std::to_string(i));
    if (!sname.empty()) {
      s << (first ? "" : "*") << sname;
      first = false;
    }
  }
  return s;
}

template <class TValue, unsigned dim>
inline std::ostream& operator<<(std::ostream& s, const Term<TValue, dim>& t) {
  if (t.tp.IsOne()) {
    if (t.a < TValue(0))
      std::cout << "-" << (-t.a);
    else
      std::cout << "+" << t.a;
  } else {
    if (t.a == TValue(1))
      s << "+" << t.tp;
    else if (t.a == -TValue(1))
      s << "-" << t.tp;
    else if (t.a < 0)
      s << "-" << (-t.a) << t.tp;
    else
      s << "+" << t.a << t.tp;
  }
  return s;
}

template <class TValue, unsigned dim>
inline std::ostream& operator<<(std::ostream& s,
                                const Function<TValue, dim>& f) {
  if (f.Empty()) {
    s << 0;
  } else {
    for (auto& t : f.terms) s << t;
  }
  return s;
}
}  // namespace multivariable
}  // namespace ext_polynomial
}  // namespace calculus
