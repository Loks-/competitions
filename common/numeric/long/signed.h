#pragma once

#include "common/base.h"
#include "common/numeric/long/unsigned.h"
#include "common/numeric/utils/abs.h"

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
  constexpr Signed() : sign(true) {}

  constexpr explicit Signed(uint32_t v) : sign(true), value(v) {}

  constexpr explicit Signed(uint64_t v) : sign(true), value(v) {}

  constexpr explicit Signed(int32_t v)
      : sign(v >= 0), value(uint32_t(Abs(v))) {}

  constexpr explicit Signed(int64_t v)
      : sign(v >= 0), value(uint64_t(Abs(v))) {}

  constexpr explicit Signed(const Unsigned& u) : sign(true), value(u) {}

  constexpr Signed(bool _sign, const Unsigned& u) : sign(_sign), value(u) {
    Normalize();
  }

  constexpr Signed(bool _sign, const TData& _data) : sign(_sign), value(_data) {
    Normalize();
  }

  constexpr void Normalize() {
    value.Normalize();
    if (value.Empty()) sign = true;
  }

  constexpr void Clear() {
    value.Clear();
    sign = true;
  }

  constexpr bool Empty() const { return value.Empty(); }

  constexpr size_t Size() const { return value.Size(); }

  constexpr bool Sign() const { return sign; }

  constexpr const Unsigned& GetUnsigned() const { return value; }

  constexpr bool Positive() const { return sign && !Empty(); }

  constexpr bool Negative() const { return !sign; }

  constexpr void swap(Signed& ls) {
    std::swap(sign, ls.sign);
    value.swap(ls.value);
  }

  constexpr bool operator==(const Signed& ls) const {
    return (sign == ls.sign) && (value == ls.value);
  }

  constexpr bool operator!=(const Signed& ls) const { return !(*this == ls); }

  constexpr bool operator<(const Signed& ls) const {
    return (sign != ls.sign) ? !sign
           : sign            ? (value < ls.value)
                             : (ls.value < value);
  }

  constexpr bool operator>(const Signed& ls) const { return ls < *this; }

  constexpr bool operator<=(const Signed& ls) const { return !(ls < *this); }

  constexpr bool operator>=(const Signed& ls) const { return !(*this < ls); }

  constexpr bool operator==(uint32_t v) const { return sign && (value == v); }

  constexpr bool operator==(uint64_t v) const { return sign && (value == v); }

  constexpr bool operator==(int32_t v) const {
    return (v < 0) ? (!sign && (value == uint32_t(-v)))
                   : (sign && (value == uint32_t(v)));
  }

  constexpr bool operator==(int64_t v) const {
    return (v < 0) ? (!sign && (value == uint64_t(-v)))
                   : (sign && (value == uint64_t(v)));
  }

  constexpr bool operator!=(uint32_t v) const { return !(*this == v); }

  constexpr bool operator!=(uint64_t v) const { return !(*this == v); }

  constexpr bool operator!=(int32_t v) const { return !(*this == v); }

  constexpr bool operator!=(int64_t v) const { return !(*this == v); }

  constexpr bool operator<(int32_t v) const { return *this < Signed(v); }

  constexpr bool operator<(uint32_t v) const { return *this < Signed(v); }

  constexpr bool operator<(int64_t v) const { return *this < Signed(v); }

  constexpr bool operator<(uint64_t v) const { return *this < Signed(v); }

  constexpr bool operator<=(int32_t v) const { return *this <= Signed(v); }

  constexpr bool operator<=(uint32_t v) const { return *this <= Signed(v); }

  constexpr bool operator<=(int64_t v) const { return *this <= Signed(v); }

  constexpr bool operator<=(uint64_t v) const { return *this <= Signed(v); }

  constexpr bool operator>(int32_t v) const { return *this > Signed(v); }

  constexpr bool operator>(uint32_t v) const { return *this > Signed(v); }

  constexpr bool operator>(int64_t v) const { return *this > Signed(v); }

  constexpr bool operator>(uint64_t v) const { return *this > Signed(v); }

  constexpr bool operator>=(int32_t v) const { return *this >= Signed(v); }

  constexpr bool operator>=(uint32_t v) const { return *this >= Signed(v); }

  constexpr bool operator>=(int64_t v) const { return *this >= Signed(v); }

  constexpr bool operator>=(uint64_t v) const { return *this >= Signed(v); }

  constexpr uint32_t ToUint32() const {
    assert(sign);
    return value.ToUint32();
  }

  constexpr uint64_t ToUint64() const {
    assert(sign);
    return value.ToUint64();
  }

  constexpr int32_t ToInt32() const {
    auto v = value.ToUint32();
    return sign ? v : -v;
  }

  constexpr int64_t ToInt64() const {
    const auto v = value.ToUint64();
    return sign ? v : -v;
  }

  constexpr const Unsigned& ToUnsigned() const {
    assert(sign);
    return value;
  }

  constexpr Signed operator-() const { return Signed(!sign, value); }

  constexpr Signed operator+(const Signed& r) const {
    return (sign == r.sign)    ? Signed(sign, value + r.value)
           : (value < r.value) ? Signed(r.sign, r.value - value)
                               : Signed(sign, value - r.value);
  }

  constexpr Signed operator-(const Signed& r) const {
    return (sign != r.sign)    ? Signed(sign, value + r.value)
           : (value < r.value) ? Signed(!sign, r.value - value)
                               : Signed(sign, value - r.value);
  }

  constexpr Signed operator+(int32_t r) const { return *this + Signed(r); }

  constexpr Signed operator+(int64_t r) const { return *this + Signed(r); }

  constexpr Signed operator+(uint32_t r) const { return *this + Signed(r); }

  constexpr Signed operator+(uint64_t r) const { return *this + Signed(r); }

  constexpr Signed operator-(int32_t r) const { return *this - Signed(r); }

  constexpr Signed operator-(int64_t r) const { return *this - Signed(r); }

  constexpr Signed operator-(uint32_t r) const { return *this - Signed(r); }

  constexpr Signed operator-(uint64_t r) const { return *this - Signed(r); }

  constexpr Signed& operator+=(const Signed& r) {
    Signed t = (*this + r);
    swap(t);
    return *this;
  }

  constexpr Signed& operator-=(const Signed& r) {
    Signed t = (*this - r);
    swap(t);
    return *this;
  }

  constexpr Signed& operator+=(int32_t r) { return *this += Signed(r); }

  constexpr Signed& operator+=(int64_t r) { return *this += Signed(r); }

  constexpr Signed& operator+=(uint32_t r) { return *this += Signed(r); }

  constexpr Signed& operator+=(uint64_t r) { return *this += Signed(r); }

  constexpr Signed& operator-=(int32_t r) { return *this -= Signed(r); }

  constexpr Signed& operator-=(int64_t r) { return *this -= Signed(r); }

  constexpr Signed& operator-=(uint32_t r) { return *this -= Signed(r); }

  constexpr Signed& operator-=(uint64_t r) { return *this -= Signed(r); }

  constexpr Signed operator*(uint32_t r) const {
    return Signed(sign, value * r);
  }

  constexpr Signed operator*(uint64_t r) const {
    return Signed(sign, value * r);
  }

  constexpr Signed operator/(uint32_t r) const {
    return Signed(sign, value / r);
  }

  constexpr Signed operator*(int32_t r) const {
    return (r < 0) ? Signed(!sign, value * uint32_t(-r))
                   : Signed(sign, value * uint32_t(r));
  }

  constexpr Signed operator*(int64_t r) const {
    return (r < 0) ? Signed(!sign, value * uint64_t(-r))
                   : Signed(sign, value * uint64_t(r));
  }

  constexpr Signed operator/(int32_t r) const {
    return (r < 0) ? Signed(!sign, value / uint32_t(r))
                   : Signed(sign, value / uint32_t(r));
  }

  constexpr int32_t operator%(uint32_t r) const {
    const auto v = value % r;
    return sign ? v : -v;
  }

  constexpr int32_t operator%(int32_t r) const {
    return *this % uint32_t(Abs(r));
  }

  constexpr Signed& operator*=(uint32_t r) {
    value *= r;
    return *this;
  }

  constexpr Signed& operator*=(int32_t r) {
    Signed t = (*this * r);
    swap(t);
    return *this;
  }

  constexpr Signed& operator*=(uint64_t r) {
    value *= r;
    return *this;
  }

  constexpr Signed& operator*=(int64_t r) {
    Signed t = (*this * r);
    swap(t);
    return *this;
  }

  constexpr Signed& operator/=(uint32_t r) {
    value /= r;
    return *this;
  }

  constexpr Signed& operator/=(int32_t r) {
    Signed t = (*this / r);
    swap(t);
    return *this;
  }

  constexpr Signed& ShiftBlocksLeft(size_t ublocks) {
    value.ShiftBlocksLeft(ublocks);
    Normalize();
    return *this;
  }

  constexpr Signed& ShiftBlocksRight(size_t ublocks) {
    value.ShiftBlocksRight(ublocks);
    return *this;
  }

  constexpr Signed& ShiftBitsLeft(size_t ubits) {
    value.ShiftBitsLeft(ubits);
    Normalize();
    return *this;
  }

  constexpr Signed& ShiftBitsRight(size_t ubits) {
    value.ShiftBitsRight(ubits);
    return *this;
  }

  constexpr Signed& operator>>=(size_t ubits) { return ShiftBitsLeft(ubits); }

  constexpr Signed& operator<<=(size_t ubits) { return ShiftBitsRight(ubits); }

  constexpr Signed operator<<(size_t ubits) const {
    Signed t(*this);
    t <<= ubits;
    return t;
  }

  constexpr Signed operator>>(size_t ubits) const {
    Signed t(*this);
    t >>= ubits;
    return t;
  }

  constexpr Signed operator/(const Signed& r) const {
    return Signed(sign == r.sign, value / r.value);
  }

  constexpr Signed operator%(const Signed& r) const {
    return Signed(sign, value % r.value);
  }

  constexpr Signed& operator/=(const Signed& r) {
    Signed t = (*this / r);
    swap(t);
    return *this;
  }

  constexpr Signed& operator%=(const Signed& r) {
    Signed t = (*this % r);
    swap(t);
    return *this;
  }

  constexpr std::vector<unsigned> ToVector(uint32_t base) const {
    return value.ToVector(base);
  }
};
}  // namespace nlong
}  // namespace numeric

using LongSigned = numeric::nlong::Signed;
