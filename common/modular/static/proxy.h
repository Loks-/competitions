#pragma once

#include "common/modular/arithmetic.h"

namespace modular {
namespace mstatic {
template <uint64_t mod, bool is_prime, bool is_32bit>
class Proxy {
 public:
  using TValue = uint64_t;
  using TBase = Arithmetic<is_prime, is_32bit, TValue>;
  using TSelf = Proxy<mod, is_prime, is_32bit>;

 public:
  consteval static bool IsModPrime() { return is_prime; }
  consteval static bool IsMod32Bits() { return is_32bit; }
  consteval static TValue GetMod() { return mod; }

  constexpr static TValue ApplyU(uint64_t value) {
    return TBase::ApplyU(value, mod);
  }

  constexpr static TValue ApplyS(int64_t value) {
    return TBase::ApplyS(value, mod);
  }

  constexpr static TValue ApplyT(TValue value) {
    return TBase::ApplyT(value, mod);
  }

  constexpr static TValue Add(TValue lvalue, TValue rvalue) {
    return TBase::Add(lvalue, rvalue, mod);
  }

  constexpr static TValue AddSafe(TValue lvalue, TValue rvalue) {
    return TBase::AddSafe(lvalue, rvalue, mod);
  }

  constexpr static TValue Sub(TValue lvalue, TValue rvalue) {
    return TBase::Sub(lvalue, rvalue, mod);
  }

  constexpr static TValue SubSafe(TValue lvalue, TValue rvalue) {
    return TBase::SubSafe(lvalue, rvalue, mod);
  }

  constexpr static TValue Minus(TValue lvalue) {
    return TBase::Minus(lvalue, mod);
  }

  constexpr static TValue MinusSafe(TValue lvalue) {
    return TBase::MinusSafe(lvalue, mod);
  }

  constexpr static TValue Mult(TValue lvalue, TValue rvalue) {
    return TBase::Mult(lvalue, rvalue, mod);
  }

  constexpr static TValue MultSafe(TValue lvalue, TValue rvalue) {
    return TBase::MultSafe(lvalue, rvalue, mod);
  }

  constexpr static TValue Inverse(TValue lvalue) {
    return TBase::Inverse(lvalue, mod);
  }

  constexpr static TValue InverseSafe(TValue lvalue) {
    return TBase::InverseSafe(lvalue, mod);
  }

  constexpr static TValue Div(TValue numerator, TValue denominator) {
    return TBase::Div(numerator, denominator, mod);
  }

  constexpr static TValue DivSafe(TValue numerator, TValue denominator) {
    return TBase::DivSafe(numerator, denominator, mod);
  }

  constexpr static TValue PowU(TValue x, uint64_t pow) {
    return TBase::PowU(x, pow, mod);
  }

  constexpr static TValue PowUSafe(TValue x, uint64_t pow) {
    return TBase::PowUSafe(x, pow, mod);
  }

  constexpr static TValue PowS(TValue x, int64_t pow) {
    return TBase::PowS(x, pow, mod);
  }

  constexpr static TValue PowSSafe(TValue x, int64_t pow) {
    return TBase::PowSSafe(x, pow, mod);
  }
};

template <uint64_t mod>
using TProxy_P32 = Proxy<mod, true, true>;
template <uint64_t mod>
using TProxy_C32 = Proxy<mod, false, true>;
}  // namespace mstatic
}  // namespace modular
