#pragma once

#include "common/base.h"
#include "common/modular/arithmetic.h"
#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/unsigned/multiplication.h"

namespace numeric {
namespace nlong {
template <bool is_prime = false>
class Modular {
 public:
  consteval static bool IsModPrime() { return is_prime; }

  constexpr static Unsigned Apply(const Unsigned& value, const Unsigned& mod) {
    return value % mod;
  }

  constexpr static Unsigned Add(const Unsigned& lvalue, const Unsigned& rvalue,
                                const Unsigned& mod) {
    Unsigned t = lvalue + rvalue;
    return (t < mod) ? t : t - mod;
  }

  constexpr static Unsigned AddSafe(const Unsigned& lvalue,
                                    const Unsigned& rvalue,
                                    const Unsigned& mod) {
    return Apply(lvalue + rvalue, mod);
  }

  constexpr static Unsigned Sub(const Unsigned& lvalue, const Unsigned& rvalue,
                                const Unsigned& mod) {
    return (rvalue <= lvalue) ? lvalue - rvalue : lvalue + mod - rvalue;
  }

  constexpr static Unsigned SubSafe(const Unsigned& lvalue,
                                    const Unsigned& rvalue,
                                    const Unsigned& mod) {
    return Apply(lvalue + mod - Apply(rvalue, mod), mod);
  }

  constexpr static Unsigned Minus(const Unsigned& value, const Unsigned& mod) {
    return (value.Empty()) ? value : mod - value;
  }

  constexpr static Unsigned MinusSafe(const Unsigned& value,
                                      const Unsigned& mod) {
    return Minus(Apply(value, mod), mod);
  }

  constexpr static Unsigned Mult(const Unsigned& lvalue, const Unsigned& rvalue,
                                 const Unsigned& mod) {
    return (lvalue * rvalue) % mod;
  }

  constexpr static Unsigned MultSafe(const Unsigned& lvalue,
                                     const Unsigned& rvalue,
                                     const Unsigned& mod) {
    return Mult(Apply(lvalue, mod), Apply(rvalue, mod), mod);
  }

  constexpr static Unsigned PowU(const Unsigned& value, uint64_t pow,
                                 const Unsigned& mod) {
    Unsigned ans(1u), x = value;
    for (; pow; pow >>= 1) {
      if (pow & 1) ans = Mult(ans, x, mod);
      x = Mult(x, x, mod);
    }
    return ans;
  }

  template <class TModularAQ = modular::TArithmetic_C32U>
  constexpr static Unsigned MergeRemainders(const Unsigned& p,
                                            const Unsigned& pr, uint32_t q,
                                            uint32_t qr) {
    return p * TModularAQ::Div(TModularAQ::Sub(qr, pr % q, q), p % q, q) + pr;
  }
};
}  // namespace nlong
}  // namespace numeric
