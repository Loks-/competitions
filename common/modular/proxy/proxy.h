#pragma once

#include "common/modular/arithmetic.h"

namespace modular {
namespace proxy {
template <bool is_prime = true, bool is_32bit = true, class _TValue = uint64_t>
class Proxy {
 public:
  using TValue = _TValue;
  using TBase = Arithmetic<is_prime, is_32bit, TValue>;
  using TSelf = Proxy<is_prime, is_32bit, TValue>;

 protected:
  TValue mod;

 public:
  consteval static bool IsModPrime() { return is_prime; }
  consteval static bool IsMod32Bits() { return is_32bit; }

  constexpr TValue GetMod() const { return mod; }

  constexpr void SetMod(TValue _mod) { mod = _mod; }

  constexpr explicit Proxy(TValue _mod = 1000000007) : mod(_mod) {}

  constexpr TValue ApplyU(uint64_t value) const {
    return TBase::ApplyU(value, mod);
  }
  constexpr TValue ApplyS(int64_t value) const {
    return TBase::ApplyS(value, mod);
  }
  constexpr TValue ApplyT(TValue value) const {
    return TBase::ApplyT(value, mod);
  }

  constexpr TValue Add(TValue lvalue, TValue rvalue) const {
    return TBase::Add(lvalue, rvalue, mod);
  }

  constexpr TValue AddSafe(TValue lvalue, TValue rvalue) const {
    return TBase::AddSafe(lvalue, rvalue, mod);
  }

  constexpr TValue Sub(TValue lvalue, TValue rvalue) const {
    return TBase::Sub(lvalue, rvalue, mod);
  }

  constexpr TValue SubSafe(TValue lvalue, TValue rvalue) const {
    return TBase::SubSafe(lvalue, rvalue, mod);
  }

  constexpr TValue Minus(TValue lvalue) const {
    return TBase::Minus(lvalue, mod);
  }

  constexpr TValue MinusSafe(TValue lvalue) const {
    return TBase::MinusSafe(lvalue, mod);
  }

  constexpr TValue Mult(TValue lvalue, TValue rvalue) const {
    return TBase::Mult(lvalue, rvalue, mod);
  }

  constexpr TValue MultSafe(TValue lvalue, TValue rvalue) const {
    return TBase::MultSafe(lvalue, rvalue, mod);
  }

  constexpr TValue Inverse(TValue lvalue) const {
    return TBase::Inverse(lvalue, mod);
  }

  constexpr TValue InverseSafe(TValue lvalue) const {
    return TBase::InverseSafe(lvalue, mod);
  }

  constexpr TValue Div(TValue numerator, TValue denominator) const {
    return TBase::Div(numerator, denominator, mod);
  }

  constexpr TValue DivSafe(TValue numerator, TValue denominator) const {
    return TBase::DivSafe(numerator, denominator, mod);
  }

  constexpr TValue PowU(TValue x, uint64_t pow) const {
    return TBase::PowU(x, pow, mod);
  }

  constexpr TValue PowUSafe(TValue x, uint64_t pow) const {
    return TBase::PowUSafe(x, pow, mod);
  }

  constexpr TValue PowS(TValue x, int64_t pow) const {
    return TBase::PowS(x, pow, mod);
  }

  constexpr TValue PowSSafe(TValue x, int64_t pow) const {
    return TBase::PowSSafe(x, pow, mod);
  }
};
}  // namespace proxy
}  // namespace modular
