#pragma once

#include "common/base.h"
#include "common/template.h"

#include <utility>

namespace modular {
// Most functions are O(1).
// Division and Inverse are O(log (mod)).
// Pow is O(log (pow)).
template <bool is_prime = true, bool is_32bit = true, class TValue = uint64_t>
class Arithmetic {
 public:
  using TSelf = Arithmetic<is_prime, is_32bit, TValue>;

  static consteval bool IsModPrime() { return is_prime; }
  static consteval bool IsMod32Bits() { return is_32bit; }

 public:
  static constexpr TValue ApplyU(uint64_t value, TValue mod) {
    return TValue(value % uint64_t(mod));
  }

  static constexpr TValue ApplyS(int64_t value, TValue mod) {
    const int64_t smod = int64_t(mod);
    return TValue(((value % smod) + smod) % smod);
  }

 protected:
  static constexpr TValue ApplyTI(TValue value, TValue mod,
                                  TFakeType<uint64_t>) {
    return ApplyU(value, mod);
  }

  static constexpr TValue ApplyTI(TValue value, TValue mod,
                                  TFakeType<int64_t>) {
    return ApplyS(value, mod);
  }

 public:
  static constexpr TValue ApplyT(TValue value, TValue mod) {
    return ApplyTI(value, mod, TFakeType<TValue>());
  }

  static constexpr TValue Add(TValue lvalue, TValue rvalue, TValue mod) {
    return (lvalue + rvalue) % mod;
  }

  static constexpr TValue AddSafe(TValue lvalue, TValue rvalue, TValue mod) {
    return ApplyT(lvalue + rvalue, mod);
  }

  static constexpr TValue Sub(TValue lvalue, TValue rvalue, TValue mod) {
    return (lvalue + mod - rvalue) % mod;
  }

  static constexpr TValue SubSafe(TValue lvalue, TValue rvalue, TValue mod) {
    return ApplyT(lvalue + mod - ApplyT(rvalue, mod), mod);
  }

  static constexpr TValue Minus(TValue value, TValue mod) {
    return (mod - value) % mod;
  }

  static constexpr TValue MinusSafe(TValue value, TValue mod) {
    return (mod - (value % mod)) % mod;
  }

  static constexpr TValue Mult_32(TValue lvalue, TValue rvalue, TValue mod) {
    return (lvalue * rvalue) % mod;
  }

  static constexpr TValue MultSafe_32(TValue lvalue, TValue rvalue,
                                      TValue mod) {
    return ApplyT((lvalue % mod) * (rvalue % mod), mod);
  }

  static constexpr TValue Sqr_32(TValue value, TValue mod) {
    return (value * value) % mod;
  }

  static constexpr TValue SqrSafe_32(TValue value, TValue rvalue, TValue mod) {
    const auto value_adj = value % mod;
    return (value_adj * value_adj) % mod;
  }

 protected:
  static constexpr int64_t Mult_64S(int64_t lvalue, int64_t rvalue,
                                    int64_t mod) {
#ifdef _INT128_SUPPORTED_
    __int128 a128 = lvalue;
    __int128 b128 = rvalue;
    a128 *= b;
    a128 %= mod;
    return int64t_t(a128);
#else
    const long double invm = ((long double)(1.0)) / mod;
    const int64_t div = int64_t(invm * lvalue * rvalue);
    int64_t res = (lvalue * rvalue) - mod * div;
    while (res < 0) res += mod;
    while (res >= mod) res -= mod;
    return res;
#endif
  }

 public:
  static constexpr TValue Mult_64(TValue lvalue, TValue rvalue, TValue mod) {
    return TValue(Mult_64S(int64_t(lvalue), int64_t(rvalue), int64_t(mod)));
  }

  static constexpr TValue MultSafe_64(TValue lvalue, TValue rvalue,
                                      TValue mod) {
    return Mult_64(lvalue % mod, rvalue % mod, mod);
  }

  static constexpr TValue Sqr_64(TValue value, TValue mod) {
    return Mult_64(value, value, mod);
  }

  static constexpr TValue SqrSafe_64(TValue value, TValue mod) {
    return Mult_64(value % mod, value % mod, mod);
  }

 protected:
  static constexpr TValue MultI(TValue lvalue, TValue rvalue, TValue mod,
                                TFakeTrue) {
    return Mult_32(lvalue, rvalue, mod);
  }

  static constexpr TValue MultI(TValue lvalue, TValue rvalue, TValue mod,
                                TFakeFalse) {
    return Mult_64(lvalue, rvalue, mod);
  }

  static constexpr TValue SqrI(TValue value, TValue mod, TFakeTrue) {
    return Sqr_32(value, mod);
  }

  static constexpr TValue SqrI(TValue value, TValue mod, TFakeFalse) {
    return Sqr_64(value, mod);
  }

 public:
  static constexpr TValue Mult(TValue lvalue, TValue rvalue, TValue mod) {
    return MultI(lvalue, rvalue, mod, TFakeBool<is_32bit>());
  }

  static constexpr TValue MultSafe(TValue lvalue, TValue rvalue, TValue mod) {
    return Mult(ApplyT(lvalue, mod), ApplyT(rvalue, mod), mod);
  }

  static constexpr TValue Sqr(TValue value, TValue mod) {
    return SqrI(value, mod, TFakeBool<is_32bit>());
  }

  static constexpr TValue SqrSafe(TValue value, TValue mod) {
    return Sqr(value % mod, mod);
  }

  static constexpr TValue Inverse_Prime(TValue value, TValue mod) {
    assert(value != 0);
    return PowU(value, mod - 2, mod);
  }

  static constexpr TValue InverseSafe_Prime(TValue value, TValue mod) {
    return Inverse_Prime(ApplyT(value, mod), mod);
  }

  static constexpr TValue Div_Prime(TValue lvalue, TValue rvalue, TValue mod) {
    return Mult(lvalue, Inverse_Prime(rvalue, mod), mod);
  }

  static constexpr TValue DivSafe_Prime(TValue lvalue, TValue rvalue,
                                        TValue mod) {
    return Div_Prime(ApplyT(lvalue), ApplyT(rvalue), mod);
  }

 protected:
  static constexpr std::pair<int64_t, int64_t> GCD_Ext(int64_t a, int64_t b) {
    if ((a % b) == 0) return std::make_pair<int64_t, int64_t>(0, 1);
    const std::pair<int64_t, int64_t> t = GCD_Ext(b, a % b);
    return std::make_pair(t.second, t.first - t.second * (a / b));
  }

  static constexpr TValue Div_CompositeS(int64_t numerator, int64_t denominator,
                                         int64_t mod) {
    const std::pair<int64_t, int64_t> dm_gcd_ext = GCD_Ext(denominator, mod);
    const int64_t dm_gcd =
        dm_gcd_ext.first * denominator + dm_gcd_ext.second * mod;
    assert((numerator % dm_gcd) == 0);
    const TValue tmod = TValue(mod);
    return Mult(ApplyS(numerator / dm_gcd, tmod),
                ApplyS(dm_gcd_ext.first, tmod), tmod);
  }

 public:
  static constexpr TValue Div_Composite(TValue numerator, TValue denominator,
                                        TValue mod) {
    return Div_CompositeS(int64_t(numerator), int64_t(denominator),
                          int64_t(mod));
  }

  static constexpr TValue DivSafe_Composite(TValue numerator,
                                            TValue denominator, TValue mod) {
    return Div_Composite(ApplyT(numerator, mod), ApplyT(denominator, mod), mod);
  }

  static constexpr TValue Inverse_Composite(TValue value, TValue mod) {
    return Div_Composite(TValue(1), value, mod);
  }

  static constexpr TValue InverseSafe_Composite(TValue value, TValue mod) {
    return Div_Composite(TValue(1), ApplyT(value, mod), mod);
  }

 protected:
  static constexpr TValue InverseI(TValue value, TValue mod, TFakeTrue) {
    return Inverse_Prime(value, mod);
  }

  static constexpr TValue InverseI(TValue value, TValue mod, TFakeFalse) {
    return Inverse_Composite(value, mod);
  }

  static constexpr TValue DivI(TValue numerator, TValue denominator, TValue mod,
                               TFakeTrue) {
    return Div_Prime(numerator, denominator, mod);
  }

  static constexpr TValue DivI(TValue numerator, TValue denominator, TValue mod,
                               TFakeFalse) {
    return Div_Composite(numerator, denominator, mod);
  }

 public:
  static constexpr TValue Inverse(TValue value, TValue mod) {
    return InverseI(value, mod, TFakeBool<is_prime>());
  }

  static constexpr TValue InverseSafe(TValue value, TValue mod) {
    return Inverse(ApplyT(value, mod), mod);
  }

  static constexpr TValue Div(TValue numerator, TValue denominator,
                              TValue mod) {
    return DivI(numerator, denominator, mod, TFakeBool<is_prime>());
  }

  static constexpr TValue DivSafe(TValue numerator, TValue denominator,
                                  TValue mod) {
    return Div(ApplyT(numerator, mod), ApplyT(denominator, mod), mod);
  }

  static constexpr TValue PowU(TValue x, uint64_t pow, TValue mod) {
    TValue ans = 1;
    for (; pow; pow >>= 1) {
      if (pow & 1) ans = Mult(ans, x, mod);
      x = Sqr(x, mod);
    }
    return ans;
  }

  static constexpr TValue PowUSafe(TValue x, uint64_t pow, TValue mod) {
    return PowU(ApplyT(x, mod), pow, mod);
  }

  static constexpr TValue PowS(TValue x, int64_t pow, TValue mod) {
    return (pow < 0) ? PowU(Inverse(x, mod), uint64_t(-pow), mod)
                     : PowU(x, uint64_t(pow), mod);
  }

  static constexpr TValue PowSSafe(TValue x, int64_t pow, TValue mod) {
    return PowS(ApplyT(x, mod), pow, mod);
  }
};

using TArithmetic_P32U = Arithmetic<true, true, uint64_t>;
using TArithmetic_C32U = Arithmetic<false, true, uint64_t>;
}  // namespace modular
