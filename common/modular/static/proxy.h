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
  static consteval bool IsModPrime() { return is_prime; }
  static consteval bool IsMod32Bits() { return is_32bit; }
  static consteval TValue GetMod() { return mod; }

  static constexpr TValue ApplyU(uint64_t value) {
    return TBase::ApplyU(value, mod);
  }

  static constexpr TValue ApplyS(int64_t value) {
    return TBase::ApplyS(value, mod);
  }

  static constexpr TValue ApplyT(TValue value) {
    return TBase::ApplyT(value, mod);
  }

  static constexpr TValue Add(TValue lvalue, TValue rvalue) {
    return TBase::Add(lvalue, rvalue, mod);
  }

  static constexpr TValue AddSafe(TValue lvalue, TValue rvalue) {
    return TBase::AddSafe(lvalue, rvalue, mod);
  }

  static constexpr TValue Sub(TValue lvalue, TValue rvalue) {
    return TBase::Sub(lvalue, rvalue, mod);
  }

  static constexpr TValue SubSafe(TValue lvalue, TValue rvalue) {
    return TBase::SubSafe(lvalue, rvalue, mod);
  }

  static constexpr TValue Minus(TValue lvalue) {
    return TBase::Minus(lvalue, mod);
  }

  static constexpr TValue MinusSafe(TValue lvalue) {
    return TBase::MinusSafe(lvalue, mod);
  }

  static constexpr TValue Mult(TValue lvalue, TValue rvalue) {
    return TBase::Mult(lvalue, rvalue, mod);
  }

  static constexpr TValue MultSafe(TValue lvalue, TValue rvalue) {
    return TBase::MultSafe(lvalue, rvalue, mod);
  }

  static constexpr TValue Sqr(TValue value) { return TBase::Sqr(value, mod); }

  static constexpr TValue SqrSafe(TValue value) {
    return TBase::SqrSafe(value, mod);
  }

  static constexpr TValue Inverse(TValue lvalue) {
    return TBase::Inverse(lvalue, mod);
  }

  static constexpr TValue InverseSafe(TValue lvalue) {
    return TBase::InverseSafe(lvalue, mod);
  }

  static constexpr TValue Div(TValue numerator, TValue denominator) {
    return TBase::Div(numerator, denominator, mod);
  }

  static constexpr TValue DivSafe(TValue numerator, TValue denominator) {
    return TBase::DivSafe(numerator, denominator, mod);
  }

  static constexpr TValue PowU(TValue x, uint64_t pow) {
    return TBase::PowU(x, pow, mod);
  }

  static constexpr TValue PowUSafe(TValue x, uint64_t pow) {
    return TBase::PowUSafe(x, pow, mod);
  }

  static constexpr TValue PowS(TValue x, int64_t pow) {
    return TBase::PowS(x, pow, mod);
  }

  static constexpr TValue PowSSafe(TValue x, int64_t pow) {
    return TBase::PowSSafe(x, pow, mod);
  }
};

template <uint64_t mod>
using TProxy_P32 = Proxy<mod, true, true>;
template <uint64_t mod>
using TProxy_C32 = Proxy<mod, false, true>;
}  // namespace mstatic
}  // namespace modular
