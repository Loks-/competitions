#pragma once

#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/io.h"
#include "common/calculus/ext_polynomial/make.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include "common/calculus/ext_polynomial/term_power.h"
#include <iostream>
#include <utility>

// Division f / (x - c)
namespace calculus {
namespace ext_polynomial {
template <class TValueF, class TValueTerm>
std::pair<Function<TValueF, TValueTerm, Term<TValueF, TValueTerm>>,
          Function<TValueF, TValueTerm, Term<TValueF, TValueTerm>>>
DivisionLinearQR(
    const Function<TValueF, TValueTerm, Term<TValueF, TValueTerm>>& f,
    const TValueF& c) {
  using TFunction = Function<TValueF, TValueTerm, Term<TValueF, TValueTerm>>;
  if (c == TValueF(0)) {
    auto fnew = f;
    for (auto& t : fnew.terms) t.tp.power -= 1;
    return {fnew, TFunction()};
  } else {
    auto ft = f;
    TFunction q;
    auto fl = MakeLinear<TValueF, TValueTerm>(c);
    while ((ft.Size() > 1) && (ft(0).tp.power < ft.terms.back().tp.power)) {
      auto t = ft.terms.back();
      t.tp.power -= 1;
      q += t;
      ft -= fl * t;
    }
    return {q, ft};
  }
}

template <class TValueF, class TValueTerm>
Function<TValueF, TValueTerm, Term<TValueF, TValueTerm>> DivisionLinear(
    const Function<TValueF, TValueTerm, Term<TValueF, TValueTerm>>& f,
    const TValueF& c, bool force_division) {
  auto p = DivisionLinearQR(f, c);
  if (p.second == TValueF(0)) return p.first;
  if (force_division) {
    std::cerr << "[Warning] Dropping remainder: " << p.second << std::endl;
    return p.first;
  } else {
    return {};
  }
}
}  // namespace ext_polynomial
}  // namespace calculus
