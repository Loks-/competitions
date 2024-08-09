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
  static consteval bool IsModPrime() { return is_prime; }

  static constexpr Unsigned Apply(const Unsigned& value, const Unsigned& mod) {
    return value % mod;
  }

  static constexpr Unsigned Add(const Unsigned& lvalue, const Unsigned& rvalue,
                                const Unsigned& mod) {
    const Unsigned t = lvalue + rvalue;
    return (t < mod) ? t : t - mod;
  }

  static constexpr Unsigned AddSafe(const Unsigned& lvalue,
                                    const Unsigned& rvalue,
                                    const Unsigned& mod) {
    return Apply(lvalue + rvalue, mod);
  }

  static constexpr Unsigned Sub(const Unsigned& lvalue, const Unsigned& rvalue,
                                const Unsigned& mod) {
    return (rvalue <= lvalue) ? lvalue - rvalue : lvalue + mod - rvalue;
  }

  static constexpr Unsigned SubSafe(const Unsigned& lvalue,
                                    const Unsigned& rvalue,
                                    const Unsigned& mod) {
    return Apply(lvalue + mod - Apply(rvalue, mod), mod);
  }

  static constexpr Unsigned Minus(const Unsigned& value, const Unsigned& mod) {
    return (value.Empty()) ? value : mod - value;
  }

  static constexpr Unsigned MinusSafe(const Unsigned& value,
                                      const Unsigned& mod) {
    return Minus(Apply(value, mod), mod);
  }

  static constexpr Unsigned Mult(const Unsigned& lvalue, const Unsigned& rvalue,
                                 const Unsigned& mod) {
    return numeric::nlong::Mult(lvalue, rvalue) % mod;
  }

  static constexpr Unsigned MultSafe(const Unsigned& lvalue,
                                     const Unsigned& rvalue,
                                     const Unsigned& mod) {
    return Mult(Apply(lvalue, mod), Apply(rvalue, mod), mod);
  }

  static constexpr Unsigned Sqr(const Unsigned& value, const Unsigned& mod) {
    return numeric::nlong::Sqr(value) % mod;
  }

  static constexpr Unsigned SqrSafe(const Unsigned& value,
                                    const Unsigned& mod) {
    return Sqr(Apply(value, mod), mod);
  }

  static constexpr Unsigned PowU(const Unsigned& value, uint64_t pow,
                                 const Unsigned& mod) {
    Unsigned ans(1u), x = value;
    for (; pow; pow >>= 1) {
      if (pow & 1) ans = Mult(ans, x, mod);
      x = Sqr(x, mod);
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
