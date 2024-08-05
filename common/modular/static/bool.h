#pragma once

#include "common/base.h"
#include "common/template.h"

namespace modular {
namespace mstatic {
class Bool {
 public:
  using TValue = bool;
  using TSelf = Bool;

 public:
  consteval static bool IsModPrime() { return true; }
  consteval static bool IsMod32Bits() { return true; }
  consteval static unsigned GetMod() { return 2u; }

 protected:
  TValue value;

 public:
  TValue Get() const { return value; }
  void SetU(uint64_t _value) { value = (_value & 1u); }
  void SetS(int64_t _value) { value = (_value & 1); }
  void SetT(TValue _value) { value = _value; }

  Bool() : value(false) {}
  Bool(bool b) : value(b) {}
  explicit Bool(uint32_t _value) { SetU(_value); }
  explicit Bool(uint64_t _value) { SetU(_value); }
  explicit Bool(int32_t _value) { SetS(_value); }
  explicit Bool(int64_t _value) { SetS(_value); }

  static Bool False() { return Bool(false); }
  static Bool True() { return Bool(true); }

  operator bool() const { return value; }

  TSelf operator+(TSelf rvalue) const { return TSelf(value != rvalue.value); }

  TSelf& operator+=(TSelf rvalue) {
    value = (value != rvalue.value);
    return *this;
  }

  TSelf operator-() const { return *this; }

  TSelf operator-(TSelf rvalue) const { return TSelf(value != rvalue.value); }

  TSelf& operator-=(TSelf rvalue) {
    value = (value != rvalue.value);
    return *this;
  }

  TSelf operator*(TSelf rvalue) const { return TSelf(value && rvalue.value); }

  TSelf& operator*=(TSelf rvalue) {
    value &= rvalue.value;
    return *this;
  }

  TSelf operator/(TSelf rvalue) const {
    FakeUse(rvalue);
    assert(rvalue.value);
    return *this;
  }

  TSelf& operator/=(TSelf rvalue) {
    FakeUse(rvalue);
    assert(rvalue.value);
    return *this;
  }

  TSelf Inverse() const {
    assert(value);
    return *this;
  }

  TSelf PowU(uint64_t pow) const { return (pow == 0) ? TSelf(true) : *this; }

  TSelf PowS(int64_t pow) const {
    assert(value || (pow >= 0));
    return (pow == 0) ? TSelf(true) : *this;
  }

  bool operator<(const TSelf& r) const { return !value && r.value; }
  bool operator==(const TSelf& r) const { return value == r.value; }
  bool operator!=(const TSelf& r) const { return value != r.value; }
};
}  // namespace mstatic
}  // namespace modular

using ModularBool = modular::mstatic::Bool;
