#pragma once

#include "common/assert_exception.h"
#include "common/calculus/ext_polynomial/function.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_bases/ln_abs.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"

namespace calculus {
namespace ext_polynomial {
namespace operators {
template <class TValue>
inline Function<TValue> Integration(const Term<TValue>& t) {
  const auto& tp = t.tp;
  switch (tp.GetType()) {
    case calculus::ext_polynomial::term_bases::Type::ONE:
      if (tp.power != -1) {
        auto tp_new = tp;
        tp_new.power += 1;
        return Term<TValue>(t.a / tp_new.power, tp_new);
      } else {
        return Term<TValue>(
            t.a, calculus::ext_polynomial::term_bases::MakeLnAbs<TValue>());
      }
    default:
      Assert(false);
      return {};
  }
}
}  // namespace operators
}  // namespace ext_polynomial
}  // namespace calculus
