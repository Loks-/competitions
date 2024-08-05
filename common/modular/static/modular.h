#pragma once

#include "common/modular/static/proxy.h"

namespace modular {
namespace mstatic {
template <uint64_t mod, bool is_prime, bool is_32bit>
class Modular {
 public:
  using TValue = uint64_t;
  using TBase = Proxy<mod, is_prime, is_32bit>;
  using TArithmetic = typename TBase::TBase;
  using TSelf = Modular<mod, is_prime, is_32bit>;

 public:
  consteval static bool IsModPrime() { return is_prime; }
  consteval static bool IsMod32Bits() { return is_32bit; }
  consteval static TValue GetMod() { return mod; }

 protected:
  TValue value;

 public:
  constexpr TValue Get() const { return value; }

  constexpr void SetU(uint64_t _value) { value = TBase::ApplyU(_value); }

  constexpr void SetS(int64_t _value) { value = TBase::ApplyS(_value); }

  constexpr void SetT(TValue _value) { value = TBase::ApplyT(_value); }

  constexpr Modular() : value(0) {}

  constexpr Modular(TValue _value) : value(TBase::ApplyT(_value)) {}

  constexpr explicit operator bool() const { return value != 0; }

  constexpr TSelf operator+(TSelf rvalue) const {
    return TBase::Add(value, rvalue.value);
  }

  constexpr TSelf& operator+=(TSelf rvalue) {
    value += rvalue.value;
    value %= mod;
    return *this;
  }

  constexpr TSelf operator-() const { return TBase::Minus(value); }

  constexpr TSelf operator-(TSelf rvalue) const {
    return TBase::Sub(value, rvalue.value);
  }

  constexpr TSelf& operator-=(TSelf rvalue) {
    value += (mod - rvalue.value);
    value %= mod;
    return *this;
  }

  constexpr TSelf operator*(TSelf rvalue) const {
    return TBase::Mult(value, rvalue.value);
  }

  constexpr TSelf& operator*=(TSelf rvalue) {
    value = TBase::Mult(value, rvalue.value);
    return *this;
  }

  constexpr TSelf operator/(TSelf rvalue) const {
    return TBase::Div(value, rvalue.value);
  }

  constexpr TSelf& operator/=(TSelf rvalue) {
    value = TBase::Div(value, rvalue.value);
    return *this;
  }

  constexpr TSelf Inverse() const { return TBase::Inverse(value); }

  constexpr TSelf PowU(uint64_t pow) const { return TBase::PowU(value, pow); }

  constexpr TSelf PowS(int64_t pow) const { return TBase::PowS(value, pow); }

  constexpr bool operator<(const TSelf& r) const { return value < r.value; }
  constexpr bool operator==(const TSelf& r) const { return value == r.value; }
  constexpr bool operator!=(const TSelf& r) const { return value != r.value; }
};
}  // namespace mstatic
}  // namespace modular
