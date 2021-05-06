#pragma once

#include "common/base.h"
#include "common/numeric/long/unsigned.h"

#include <algorithm>

namespace numeric {
namespace nlong {
class Signed {
 public:
  using TData = Unsigned::TData;

 protected:
  bool sign;  // true if positive or zero
  Unsigned value;

 public:
  Signed() : sign(true) {}

  explicit Signed(uint32_t v) : sign(true), value(v) {}

  explicit Signed(uint64_t v) : sign(true), value(v) {}

  explicit Signed(int32_t v) : sign(v >= 0), value(uint32_t(abs(v))) {}

  explicit Signed(int64_t v) : sign(v >= 0), value(uint64_t(abs(v))) {}

  explicit Signed(const Unsigned& u) : sign(true), value(u) {}

  Signed(bool _sign, const Unsigned& u) : sign(_sign), value(u) { Normalize(); }

  Signed(bool _sign, const TData& _data) : sign(_sign), value(_data) {
    Normalize();
  }

  void Normalize() {
    value.Normalize();
    if (value.Empty()) sign = true;
  }

  void Clear() {
    value.Clear();
    sign = true;
  }

  bool Empty() const { return value.Empty(); }
  size_t Size() const { return value.Size(); }
  bool Sign() const { return sign; }
  const Unsigned& GetUnsigned() const { return value; }

  bool Positive() const { return sign && !Empty(); }
  bool Negative() const { return !sign; }

  void swap(Signed& ls) {
    std::swap(sign, ls.sign);
    value.swap(ls.value);
  }

  bool operator==(const Signed& ls) const {
    return (sign == ls.sign) && (value == ls.value);
  }
  bool operator!=(const Signed& ls) const { return !(*this == ls); }

  bool operator<(const Signed& ls) const {
    return (sign != ls.sign) ? !sign : sign ? (value < ls.value)
                                            : (ls.value < value);
  }

  bool operator>(const Signed& ls) const { return ls < *this; }
  bool operator<=(const Signed& ls) const { return !(ls < *this); }
  bool operator>=(const Signed& ls) const { return !(*this < ls); }

  bool operator==(uint32_t v) const { return sign && (value == v); }
  bool operator==(uint64_t v) const { return sign && (value == v); }

  bool operator==(int32_t v) const {
    return (v < 0) ? (!sign && (value == uint32_t(-v)))
                   : (sign && (value == uint32_t(v)));
  }

  bool operator==(int64_t v) const {
    return (v < 0) ? (!sign && (value == uint64_t(-v)))
                   : (sign && (value == uint64_t(v)));
  }

  bool operator!=(uint32_t v) const { return !(*this == v); }
  bool operator!=(uint64_t v) const { return !(*this == v); }
  bool operator!=(int32_t v) const { return !(*this == v); }
  bool operator!=(int64_t v) const { return !(*this == v); }

  bool operator<(int32_t v) const { return *this < Signed(v); }
  bool operator<(uint32_t v) const { return *this < Signed(v); }
  bool operator<(int64_t v) const { return *this < Signed(v); }
  bool operator<(uint64_t v) const { return *this < Signed(v); }

  bool operator<=(int32_t v) const { return *this <= Signed(v); }
  bool operator<=(uint32_t v) const { return *this <= Signed(v); }
  bool operator<=(int64_t v) const { return *this <= Signed(v); }
  bool operator<=(uint64_t v) const { return *this <= Signed(v); }

  bool operator>(int32_t v) const { return *this > Signed(v); }
  bool operator>(uint32_t v) const { return *this > Signed(v); }
  bool operator>(int64_t v) const { return *this > Signed(v); }
  bool operator>(uint64_t v) const { return *this > Signed(v); }

  bool operator>=(int32_t v) const { return *this >= Signed(v); }
  bool operator>=(uint32_t v) const { return *this >= Signed(v); }
  bool operator>=(int64_t v) const { return *this >= Signed(v); }
  bool operator>=(uint64_t v) const { return *this >= Signed(v); }

  uint32_t ToUint32() const {
    assert(sign);
    return value.ToUint32();
  }

  uint64_t ToUint64() const {
    assert(sign);
    return value.ToUint64();
  }

  int32_t ToInt32() const {
    auto v = value.ToUint32();
    return sign ? v : -v;
  }

  int64_t ToInt64() const {
    auto v = value.ToUint64();
    return sign ? v : -v;
  }

  const Unsigned& ToUnsigned() const {
    assert(sign);
    return value;
  }

  Signed operator-() const { return Signed(!sign, value); }

  Signed operator+(const Signed& r) const {
    return (sign == r.sign)
               ? Signed(sign, value + r.value)
               : (value < r.value) ? Signed(r.sign, r.value - value)
                                   : Signed(sign, value - r.value);
  }

  Signed operator-(const Signed& r) const {
    return (sign != r.sign) ? Signed(sign, value + r.value)
                            : (value < r.value) ? Signed(!sign, r.value - value)
                                                : Signed(sign, value - r.value);
  }

  Signed operator+(int32_t r) const { return *this + Signed(r); }
  Signed operator+(int64_t r) const { return *this + Signed(r); }
  Signed operator+(uint32_t r) const { return *this + Signed(r); }
  Signed operator+(uint64_t r) const { return *this + Signed(r); }

  Signed operator-(int32_t r) const { return *this - Signed(r); }
  Signed operator-(int64_t r) const { return *this - Signed(r); }
  Signed operator-(uint32_t r) const { return *this - Signed(r); }
  Signed operator-(uint64_t r) const { return *this - Signed(r); }

  Signed& operator+=(const Signed& r) {
    Signed t = (*this + r);
    swap(t);
    return *this;
  }

  Signed& operator-=(const Signed& r) {
    Signed t = (*this - r);
    swap(t);
    return *this;
  }

  Signed& operator+=(int32_t r) { return *this += Signed(r); }
  Signed& operator+=(int64_t r) { return *this += Signed(r); }
  Signed& operator+=(uint32_t r) { return *this += Signed(r); }
  Signed& operator+=(uint64_t r) { return *this += Signed(r); }

  Signed& operator-=(int32_t r) { return *this -= Signed(r); }
  Signed& operator-=(int64_t r) { return *this -= Signed(r); }
  Signed& operator-=(uint32_t r) { return *this -= Signed(r); }
  Signed& operator-=(uint64_t r) { return *this -= Signed(r); }

  Signed operator*(uint32_t r) const { return Signed(sign, value * r); }
  Signed operator*(uint64_t r) const { return Signed(sign, value * r); }
  Signed operator/(uint32_t r) const { return Signed(sign, value / r); }

  Signed operator*(int32_t r) const {
    return (r < 0) ? Signed(!sign, value * uint32_t(-r))
                   : Signed(sign, value * uint32_t(r));
  }

  Signed operator*(int64_t r) const {
    return (r < 0) ? Signed(!sign, value * uint64_t(-r))
                   : Signed(sign, value * uint64_t(r));
  }

  Signed operator/(int32_t r) const {
    return (r < 0) ? Signed(!sign, value / uint32_t(r))
                   : Signed(sign, value / uint32_t(r));
  }

  int32_t operator%(uint32_t r) const {
    auto v = value % r;
    return sign ? v : -v;
  }

  int32_t operator%(int32_t r) const { return *this % uint32_t(abs(r)); }

  Signed& operator*=(uint32_t r) {
    value *= r;
    return *this;
  }

  Signed& operator*=(int32_t r) {
    Signed t = (*this * r);
    swap(t);
    return *this;
  }

  Signed& operator*=(uint64_t r) {
    value *= r;
    return *this;
  }

  Signed& operator*=(int64_t r) {
    Signed t = (*this * r);
    swap(t);
    return *this;
  }

  Signed& operator/=(uint32_t r) {
    value /= r;
    return *this;
  }

  Signed& operator/=(int32_t r) {
    Signed t = (*this / r);
    swap(t);
    return *this;
  }

  Signed& ShiftBlocksLeft(size_t ublocks) {
    value.ShiftBlocksLeft(ublocks);
    Normalize();
    return *this;
  }

  Signed& ShiftBlocksRight(size_t ublocks) {
    value.ShiftBlocksRight(ublocks);
    return *this;
  }

  Signed& ShiftBitsLeft(size_t ubits) {
    value.ShiftBitsLeft(ubits);
    Normalize();
    return *this;
  }

  Signed& ShiftBitsRight(size_t ubits) {
    value.ShiftBitsRight(ubits);
    return *this;
  }

  Signed& operator>>=(size_t ubits) { return ShiftBitsLeft(ubits); }
  Signed& operator<<=(size_t ubits) { return ShiftBitsRight(ubits); }

  Signed operator<<(size_t ubits) const {
    Signed t(*this);
    t <<= ubits;
    return t;
  }

  Signed operator>>(size_t ubits) const {
    Signed t(*this);
    t >>= ubits;
    return t;
  }

  Signed operator/(const Signed& r) const {
    return Signed(sign == r.sign, value / r.value);
  }

  Signed operator%(const Signed& r) const {
    return Signed(sign, value % r.value);
  }

  Signed& operator/=(const Signed& r) {
    Signed t = (*this / r);
    swap(t);
    return *this;
  }

  Signed& operator%=(const Signed& r) {
    Signed t = (*this % r);
    swap(t);
    return *this;
  }

  std::vector<unsigned> ToVector(uint32_t base) const {
    return value.ToVector(base);
  }
};
}  // namespace nlong
}  // namespace numeric

using LongSigned = numeric::nlong::Signed;
