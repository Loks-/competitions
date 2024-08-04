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
  TValue Get() const { return value; }
  void SetU(uint64_t _value) { value = TBase::ApplyU(_value); }
  void SetS(int64_t _value) { value = TBase::ApplyS(_value); }
  void SetT(TValue _value) { value = TBase::ApplyT(_value); }

  Modular() : value(0) {}
  Modular(TValue _value) : value(TBase::ApplyT(_value)) {}

  explicit operator bool() const { return value != 0; }

  TSelf operator+(TSelf rvalue) const {
    return TBase::Add(value, rvalue.value);
  }

  TSelf& operator+=(TSelf rvalue) {
    value += rvalue.value;
    value %= mod;
    return *this;
  }

  TSelf operator-() const { return TBase::Minus(value); }

  TSelf operator-(TSelf rvalue) const {
    return TBase::Sub(value, rvalue.value);
  }

  TSelf& operator-=(TSelf rvalue) {
    value += (mod - rvalue.value);
    value %= mod;
    return *this;
  }

  TSelf operator*(TSelf rvalue) const {
    return TBase::Mult(value, rvalue.value);
  }

  TSelf& operator*=(TSelf rvalue) {
    value = TBase::Mult(value, rvalue.value);
    return *this;
  }

  TSelf operator/(TSelf rvalue) const {
    return TBase::Div(value, rvalue.value);
  }

  TSelf& operator/=(TSelf rvalue) {
    value = TBase::Div(value, rvalue.value);
    return *this;
  }

  TSelf Inverse() const { return TBase::Inverse(value); }

  TSelf PowU(uint64_t pow) const { return TBase::PowU(value, pow); }

  TSelf PowS(int64_t pow) const { return TBase::PowS(value, pow); }

  bool operator<(const TSelf& r) const { return value < r.value; }
  bool operator==(const TSelf& r) const { return value == r.value; }
  bool operator!=(const TSelf& r) const { return value != r.value; }
};
}  // namespace mstatic
}  // namespace modular
