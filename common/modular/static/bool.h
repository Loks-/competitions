#pragma once

#include "common/base.h"

namespace modular {
namespace mstatic {
class Bool {
 public:
  using TValue = bool;
  using TSelf = Bool;

 public:
  static consteval bool IsModPrime() { return true; }
  static consteval bool IsMod32Bits() { return true; }
  static consteval unsigned GetMod() { return 2u; }

 protected:
  TValue value;

 public:
  constexpr TValue Get() const { return value; }
  constexpr void SetU(uint64_t _value) { value = (_value & 1u); }
  constexpr void SetS(int64_t _value) { value = (_value & 1); }
  constexpr void SetT(TValue _value) { value = _value; }

  constexpr Bool() : value(false) {}
  constexpr explicit Bool(bool b) : value(b) {}
  constexpr explicit Bool(uint32_t _value) { SetU(_value); }
  constexpr explicit Bool(uint64_t _value) { SetU(_value); }
  constexpr explicit Bool(int32_t _value) { SetS(_value); }
  constexpr explicit Bool(int64_t _value) { SetS(_value); }

  static consteval Bool False() { return Bool(false); }
  static consteval Bool True() { return Bool(true); }

  constexpr operator bool() const { return value; }

  constexpr TSelf operator+(TSelf rvalue) const {
    return TSelf(value != rvalue.value);
  }

  constexpr TSelf& operator+=(TSelf rvalue) {
    value = (value != rvalue.value);
    return *this;
  }

  constexpr TSelf operator-() const { return *this; }

  constexpr TSelf operator-(TSelf rvalue) const {
    return TSelf(value != rvalue.value);
  }

  constexpr TSelf& operator-=(TSelf rvalue) {
    value = (value != rvalue.value);
    return *this;
  }

  constexpr TSelf operator*(TSelf rvalue) const {
    return TSelf(value && rvalue.value);
  }

  constexpr TSelf& operator*=(TSelf rvalue) {
    value = (value && rvalue.value);
    return *this;
  }

  constexpr TSelf operator/([[maybe_unused]] TSelf rvalue) const {
    assert(rvalue.value);
    return *this;
  }

  constexpr TSelf& operator/=([[maybe_unused]] TSelf rvalue) {
    assert(rvalue.value);
    return *this;
  }

  constexpr TSelf Inverse() const {
    assert(value);
    return *this;
  }

  constexpr TSelf PowU(uint64_t pow) const {
    return (pow == 0) ? TSelf(true) : *this;
  }

  constexpr TSelf PowS(int64_t pow) const {
    assert(value || (pow >= 0));
    return (pow == 0) ? TSelf(true) : *this;
  }

  constexpr bool operator<(const TSelf& r) const { return !value && r.value; }
  constexpr bool operator==(const TSelf& r) const { return value == r.value; }
  constexpr bool operator!=(const TSelf& r) const { return value != r.value; }
};
}  // namespace mstatic
}  // namespace modular

using ModularBool = modular::mstatic::Bool;
