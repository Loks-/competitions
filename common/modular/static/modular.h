#pragma once

#include "common/modular/static/modular_proxy.h"

template <uint64_t mod = 1000000007, bool is_prime = true, bool is_32bit = true>
class Modular {
 public:
  using TValue = uint64_t;
  using TBase = ModularStaticProxy<mod, is_prime, is_32bit>;
  using TArithmetic = typename TBase::TBase;
  using TSelf = Modular<mod, is_prime, is_32bit>;

 public:
  static bool IsModPrime() { return is_prime; }
  static bool IsMod32Bits() { return is_32bit; }
  static TValue GetMod() { return mod; }

 protected:
  TValue value;

 public:
  TValue Get() const { return value; }
  void SetU(uint64_t _value) { value = TBase::ApplyU(_value); }
  void SetS(int64_t _value) { value = TBase::ApplyS(_value); }
  void SetT(TValue _value) { value = TBase::ApplyT(_value); }

  Modular() : value(0) {}
  Modular(const TSelf& s) : value(s.value) {}
  Modular(TValue _value) : value(TBase::ApplyT(_value)) {}

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

template <uint64_t mod>
using TModular_P32 = Modular<mod, true, true>;
template <uint64_t mod>
using TModular_C32 = Modular<mod, false, true>;
