#pragma once

#include "common/base.h"
#include <algorithm>
#include <vector>

namespace numeric {
namespace nlong {
class Unsigned {
 public:
  using TData = std::vector<uint32_t>;

 protected:
  TData data;

 public:
  using iterator = uint32_t*;
  using const_iterator = const uint32_t*;

  Unsigned() {}

  Unsigned(uint32_t u) {
    if (u) data.push_back(u);
  }

  Unsigned(uint64_t u) {
    data.push_back(uint32_t(u));
    data.push_back(uint32_t(u >> 32));
    Normalize();
  }

  Unsigned(const TData& _data) : data(_data) {}

  void Normalize() {
    for (; (data.size() > 0) && (data.back() == 0);) data.pop_back();
  }

  void Clear() { data.clear(); }
  bool Empty() const { return data.empty(); }
  size_t Size() const { return data.size(); }
  const TData& Data() const { return data; }

  iterator begin() { return &data.front(); }
  const_iterator begin() const { return &data.front(); }
  iterator end() { return begin() + data.size(); }
  const_iterator end() const { return begin() + data.size(); }
  void swap(Unsigned& lu) { data.swap(lu.data); }

  bool operator==(const Unsigned& lu) const { return data == lu.data; }
  bool operator!=(const Unsigned& lu) const { return data != lu.data; }

  bool operator<(const Unsigned& lu) const {
    if (Size() < lu.Size()) return true;
    if (lu.Size() < Size()) return false;
    for (size_t i = Size(); i--;) {
      if (data[i] < lu.data[i]) return true;
      if (data[i] > lu.data[i]) return false;
    }
    return false;
  }

  bool operator<=(const Unsigned& lu) const {
    if (Size() < lu.Size()) return true;
    if (lu.Size() < Size()) return false;
    for (size_t i = Size(); i--;) {
      if (data[i] < lu.data[i]) return true;
      if (data[i] > lu.data[i]) return false;
    }
    return true;
  }

  bool operator>(const Unsigned& lu) const { return !(lu <= (*this)); }
  bool operator>=(const Unsigned& lu) const { return !(lu < (*this)); }

  Unsigned operator+(uint64_t u) const {
    if (u == 0) return (*this);
    Unsigned lu;
    lu.data.reserve(data.size());
    uint64_t t64 = u;
    for (uint32_t ud : data) {
      t64 += ud;
      lu.data.push_back(uint32_t(t64));
      t64 >>= 32;
    }
    if (t64) lu.data.push_back(uint32_t(t64));
    return lu;
  }

  Unsigned operator*(uint32_t u) const {
    if (u == 0) return Unsigned();
    Unsigned lu;
    lu.data.reserve(data.size() + 1);
    uint64_t u64 = u, t64 = 0;
    for (uint32_t ud : data) {
      t64 += u64 * ud;
      lu.data.push_back(uint32_t(t64));
      t64 >>= 32;
    }
    if (t64) lu.data.push_back(uint32_t(t64));
    return lu;
  }

  Unsigned operator*(uint64_t u) const {
    if (u == 0) return Unsigned();
    Unsigned lu1 = *this * uint32_t(u);
    u >>= 32;
    if (u == 0) return lu1;
    Unsigned lu2 = *this * uint32_t(u);
    lu2.data.insert(lu2.data.begin(), 0);
    return lu1 + lu2;
  }

  Unsigned operator/(uint32_t u) const {
    assert(u);
    if (data.empty()) return Unsigned();
    Unsigned lu;
    lu.data.reserve(data.size());
    uint64_t u64 = u, t64 = data.back();
    if (t64 >= u64) {
      lu.data.push_back(uint32_t(t64 / u64));
      t64 %= u64;
    }
    for (auto p = end() - 2, pb = begin(); p >= pb;) {
      t64 = ((t64 << 32) + *p--);
      lu.data.push_back(uint32_t(t64 / u64));
      t64 %= u64;
    }
    if (lu.Size() > 1) std::reverse(lu.begin(), lu.end());
    return lu;
  }

  uint32_t operator%(uint32_t u) const {
    assert(u);
    if (data.empty()) return 0;
    if ((u & (u - 1)) == 0) return (data[0] % u);
    uint64_t u64 = u, t64 = data.back() % u64;
    for (auto p = end() - 2, pb = begin(); p >= pb;)
      t64 = ((t64 << 32) + *p--) % u64;
    return uint32_t(t64);
  }

  Unsigned operator+(const Unsigned& r) const {
    if (Empty()) return r;
    if (r.Empty()) return *this;
    Unsigned lu;
    size_t l = std::max(data.size(), r.data.size());
    lu.data.reserve(l);
    uint64_t t64 = 0;
    for (size_t i = 0; i < l; ++i) {
      t64 += (i < data.size()) ? data[i] : 0;
      t64 += (i < r.data.size()) ? r.data[i] : 0;
      lu.data.push_back(uint32_t(t64));
      t64 >>= 32;
    }
    if (t64) lu.data.push_back(uint32_t(t64));
    return lu;
  }

  Unsigned operator-(const Unsigned& r) const {
    if (r.Empty()) return *this;
    assert(r <= *this);
    Unsigned lu;
    size_t l = data.size();
    lu.data.reserve(l);
    int64_t i64 = 0;
    for (size_t i = 0; i < l; ++i) {
      i64 += int64_t(data[i]);
      i64 -= (i < r.data.size()) ? int64_t(r.data[i]) : 0;
      lu.data.push_back(uint32_t(i64));
      i64 >>= 32;
    }
    assert(!i64);
    lu.Normalize();
    return lu;
  }

  Unsigned& operator+=(uint64_t u) {
    Unsigned t = (*this + u);
    swap(t);
    return *this;
  }

  Unsigned& operator*=(uint32_t u) {
    Unsigned t = (*this * u);
    swap(t);
    return *this;
  }

  Unsigned& operator*=(uint64_t u) {
    Unsigned t = (*this * u);
    swap(t);
    return *this;
  }

  Unsigned& operator/=(uint32_t u) {
    Unsigned t = (*this / u);
    swap(t);
    return *this;
  }

  Unsigned& operator+=(const Unsigned& r) {
    Unsigned t = (*this + r);
    swap(t);
    return *this;
  }

  Unsigned& operator-=(const Unsigned& r) {
    Unsigned t = (*this - r);
    swap(t);
    return *this;
  }

  Unsigned& ShiftBlocksLeft(unsigned ublocks) {
    if (ublocks) data.erase(data.begin(), data.begin() + ublocks);
    return *this;
  }

  Unsigned& ShiftBlocksRight(unsigned ublocks) {
    if (ublocks && !Empty()) data.insert(data.begin(), ublocks, 0);
    return *this;
  }

  Unsigned& ShiftBitsLeft(unsigned ubits) {
    ShiftBlocksLeft(ubits / 32);
    ubits %= 32;
    if (ubits && !Empty()) {
      for (unsigned i = 0, ie = Size() - 1; i < ie; ++i) {
        data[i] = (data[i] >> ubits) | (data[i + 1] << (32 - ubits));
      }
      data.back() >>= ubits;
    }
    Normalize();
    return *this;
  }

  Unsigned& ShiftBitsRight(unsigned ubits) {
    ShiftBlocksRight(ubits / 32);
    ubits %= 32;
    if (ubits && !Empty()) {
      unsigned tail = data.back() >> (32 - ubits);
      for (unsigned i = Size() - 1; i; --i) {
        data[i] = (data[i] << ubits) | (data[i - 1] >> (32 - ubits));
      }
      data[0] <<= ubits;
      if (tail) data.push_back(tail);
    }
    return *this;
  }

  Unsigned& operator>>=(unsigned ubits) { return ShiftBitsLeft(ubits); }
  Unsigned& operator<<=(unsigned ubits) { return ShiftBitsRight(ubits); }

  Unsigned operator<<(unsigned ubits) const {
    Unsigned t(*this);
    t <<= ubits;
    return t;
  }

  Unsigned operator>>(unsigned ubits) const {
    Unsigned t(*this);
    t >>= ubits;
    return t;
  }

  Unsigned operator/(const Unsigned& r) const {
    assert(!r.Empty());
    if (Empty()) return Unsigned();
    Unsigned tl(*this), tr(r), lu;
    unsigned total_shift = 1 + std::max(Size(), r.Size()) - r.Size();
    tr.ShiftBlocksRight(total_shift);
    for (unsigned i = 0; i < 32 * total_shift; ++i) {
      lu.ShiftBitsRight(1);
      tr.ShiftBitsLeft(1);
      if (tr <= tl) {
        tl -= tr;
        lu += 1;
      }
    }
    return lu;
  }

  Unsigned operator%(const Unsigned& r) const {
    assert(!r.Empty());
    if (Empty()) return Unsigned();
    Unsigned tl(*this), tr(r);
    unsigned total_shift = 1 + std::max(Size(), r.Size()) - r.Size();
    tr.ShiftBlocksRight(total_shift);
    for (unsigned i = 0; i < 32 * total_shift; ++i) {
      tr.ShiftBitsLeft(1);
      if (tr <= tl) {
        tl -= tr;
      }
    }
    return tl;
  }

  Unsigned& operator/=(const Unsigned& r) {
    Unsigned t = (*this / r);
    swap(t);
    return *this;
  }

  Unsigned& operator%=(const Unsigned& r) {
    Unsigned t = (*this % r);
    swap(t);
    return *this;
  }

  std::vector<unsigned> ToVector(uint32_t base) const {
    Unsigned t(*this);
    std::vector<unsigned> v;
    for (; !t.Empty(); t /= base) v.push_back(unsigned(t % base));
    return v;
  }
};
}  // namespace nlong
}  // namespace numeric

using LongUnsigned = numeric::nlong::Unsigned;
