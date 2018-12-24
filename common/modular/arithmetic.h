#pragma once

#include "../base.h"
#include "../template.h"
#include <utility>

// Most functions are O(1).
// Division and Inverse are O(log (mod)).
// Pow is O(log (pow)).
template <bool is_prime = true, bool is_32bit = true, class _TValue = uint64_t>
class ModularArithmetic {
 public:
  using TValue = _TValue;
  using TSelf = ModularArithmetic<is_prime, is_32bit, TValue>;

  static bool IsModPrime() { return is_prime; }
  static bool IsMod32Bits() { return is_32bit; }

  static TValue ApplyU(uint64_t value, TValue mod) {
    return TValue(value % uint64_t(mod));
  }

  static TValue ApplyS(int64_t value, TValue mod) {
    int64_t smod = int64_t(mod);
    return TValue(((value % smod) + smod) % smod);
  }

 protected:
  static TValue ApplyTI(TValue value, TValue mod, TFakeType<uint64_t>) {
    return ApplyU(value, mod);
  }

  static TValue ApplyTI(TValue value, TValue mod, TFakeType<int64_t>) {
    return ApplyS(value, mod);
  }

 public:
  static TValue ApplyT(TValue value, TValue mod) {
    return ApplyTI(value, mod, TFakeType<TValue>());
  }

  static TValue Add(TValue lvalue, TValue rvalue, TValue mod) {
    return (lvalue + rvalue) % mod;
  }

  static TValue AddSafe(TValue lvalue, TValue rvalue, TValue mod) {
    return ApplyT(lvalue + rvalue, mod);
  }

  static TValue Sub(TValue lvalue, TValue rvalue, TValue mod) {
    return (lvalue + mod - rvalue) % mod;
  }

  static TValue SubSafe(TValue lvalue, TValue rvalue, TValue mod) {
    return ApplyT(lvalue + mod - ApplyT(rvalue, mod), mod);
  }

  static TValue Minus(TValue value, TValue mod) { return (mod - value) % mod; }

  static TValue MinusSafe(TValue value, TValue mod) {
    return (mod - (value % mod)) % mod;
  }

  static TValue Mult_32(TValue lvalue, TValue rvalue, TValue mod) {
    return (lvalue * rvalue) % mod;
  }

  static TValue MultSafe_32(TValue lvalue, TValue rvalue, TValue mod) {
    return ApplyT((lvalue % mod) * (rvalue % mod), mod);
  }

 protected:
  static int64_t Mult_64S(int64_t lvalue, int64_t rvalue, int64_t mod) {
#ifdef _INT128_SUPPORTED_
    __int128 a128 = lvalue;
    __int128 b128 = rvalue;
    a128 *= b;
    a128 %= mod;
    return int64t_t(a128);
#else
    long double invm = ((long double)(1.0)) / mod;
    int64_t div = int64_t(invm * lvalue * rvalue);
    int64_t res = (lvalue * rvalue) - mod * div;
    while (res < 0) res += mod;
    while (res >= mod) res -= mod;
    return res;
#endif
  }

 public:
  static TValue Mult_64(TValue lvalue, TValue rvalue, TValue mod) {
    return TValue(Mult_64S(int64_t(lvalue), int64_t(rvalue), int64_t(mod)));
  }

  static TValue MultSafe_64(TValue lvalue, TValue rvalue, TValue mod) {
    return Mult_64(lvalue % mod, rvalue % mod, mod);
  }

 protected:
  static TValue MultI(TValue lvalue, TValue rvalue, TValue mod, TFakeTrue) {
    return Mult_32(lvalue, rvalue, mod);
  }

  static TValue MultI(TValue lvalue, TValue rvalue, TValue mod, TFakeFalse) {
    return Mult_64(lvalue, rvalue, mod);
  }

 public:
  static TValue Mult(TValue lvalue, TValue rvalue, TValue mod) {
    return MultI(lvalue, rvalue, mod, TFakeBool<is_32bit>());
  }

  static TValue MultSafe(TValue lvalue, TValue rvalue, TValue mod) {
    return Mult(ApplyT(lvalue, mod), ApplyT(rvalue, mod), mod);
  }

  static TValue Inverse_Prime(TValue value, TValue mod) {
    assert(value != 0);
    return PowU(value, mod - 2, mod);
  }

  static TValue InverseSafe_Prime(TValue value, TValue mod) {
    return Inverse_Prime(ApplyT(value, mod), mod);
  }

  static TValue Div_Prime(TValue lvalue, TValue rvalue, TValue mod) {
    return Mult(lvalue, Inverse_Prime(rvalue, mod), mod);
  }

  static TValue DivSafe_Prime(TValue lvalue, TValue rvalue, TValue mod) {
    return Div_Prime(ApplyT(lvalue), ApplyT(rvalue), mod);
  }

 protected:
  static std::pair<int64_t, int64_t> GCD_Ext(int64_t a, int64_t b) {
    if ((a % b) == 0) return std::make_pair<int64_t, int64_t>(0, 1);
    std::pair<int64_t, int64_t> t = GCD_Ext(b, a % b);
    return std::make_pair(t.second, t.first - t.second * (a / b));
  }

  static TValue Div_CompositeS(int64_t numerator, int64_t denominator,
                               int64_t mod) {
    std::pair<int64_t, int64_t> dm_gcd_ext = GCD_Ext(denominator, mod);
    int64_t dm_gcd = dm_gcd_ext.first * denominator + dm_gcd_ext.second * mod;
    assert((numerator % dm_gcd) == 0);
    TValue tmod = TValue(mod);
    return Mult(ApplyS(numerator / dm_gcd, tmod),
                ApplyS(dm_gcd_ext.first, tmod), tmod);
  }

 public:
  static TValue Div_Composite(TValue numerator, TValue denominator,
                              TValue mod) {
    return Div_CompositeS(int64_t(numerator), int64_t(denominator),
                          int64_t(mod));
  }

  static TValue DivSafe_Composite(TValue numerator, TValue denominator,
                                  TValue mod) {
    return Div_Composite(ApplyT(numerator, mod), ApplyT(denominator, mod), mod);
  }

  static TValue Inverse_Composite(TValue value, TValue mod) {
    return Div_Composite(TValue(1), value, mod);
  }

  static TValue InverseSafe_Composite(TValue value, TValue mod) {
    return Div_Composite(TValue(1), ApplyT(value, mod), mod);
  }

 protected:
  static TValue InverseI(TValue value, TValue mod, TFakeTrue) {
    return Inverse_Prime(value, mod);
  }

  static TValue InverseI(TValue value, TValue mod, TFakeFalse) {
    return Inverse_Composite(value, mod);
  }

  static TValue DivI(TValue numerator, TValue denominator, TValue mod,
                     TFakeTrue) {
    return Div_Prime(numerator, denominator, mod);
  }

  static TValue DivI(TValue numerator, TValue denominator, TValue mod,
                     TFakeFalse) {
    return Div_Composite(numerator, denominator, mod);
  }

 public:
  static TValue Inverse(TValue value, TValue mod) {
    return InverseI(value, mod, TFakeBool<is_prime>());
  }

  static TValue InverseSafe(TValue value, TValue mod) {
    return Inverse(ApplyT(value, mod), mod);
  }

  static TValue Div(TValue numerator, TValue denominator, TValue mod) {
    return DivI(numerator, denominator, mod, TFakeBool<is_prime>());
  }

  static TValue DivSafe(TValue numerator, TValue denominator, TValue mod) {
    return Div(ApplyT(numerator, mod), ApplyT(denominator, mod), mod);
  }

  static TValue PowU(TValue x, uint64_t pow, TValue mod) {
    TValue ans = 1;
    for (; pow; pow >>= 1) {
      if (pow & 1) ans = Mult(ans, x, mod);
      x = Mult(x, x, mod);
    }
    return ans;
  }

  static TValue PowUSafe(TValue x, uint64_t pow, TValue mod) {
    return PowU(ApplyT(x, mod), pow, mod);
  }

  static TValue PowS(TValue x, int64_t pow, TValue mod) {
    return (pow < 0) ? PowU(Inverse(x, mod), uint64_t(-pow), mod)
                     : PowU(x, uint64_t(pow), mod);
  }

  static TValue PowSSafe(TValue x, int64_t pow, TValue mod) {
    return PowS(ApplyT(x, mod), pow, mod);
  }
};

using TModularArithmetic_P32U = ModularArithmetic<true, true, uint64_t>;
using TModularArithmetic_C32U = ModularArithmetic<false, true, uint64_t>;
