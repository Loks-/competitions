#pragma once

#include "common/base.h"
#include "common/template.h"

#include <array>
#include <initializer_list>

namespace la {
template <class TTValue, unsigned size>
class VectorStaticSize {
 public:
  using TValue = TTValue;
  using TData = std::array<TValue, size>;
  using TSelf = VectorStaticSize<TValue, size>;

  using iterator = TValue*;
  using const_iterator = const TValue*;

 protected:
  TData data;

 public:
  constexpr void Fill(const TValue& v) { data.fill(v); }

  constexpr void Clear() { Fill(TValue()); }

  static constexpr unsigned Size() { return size; }

  constexpr const TData& GetData() const { return data; }

  constexpr VectorStaticSize() { Clear(); }

  constexpr explicit VectorStaticSize(unsigned vsize) {
    assert(vsize == size);
    FakeUse(vsize);
    Clear();
  }

  constexpr VectorStaticSize(unsigned vsize, const TValue& v) {
    assert(vsize == size);
    FakeUse(vsize);
    Fill(v);
  }

  constexpr explicit VectorStaticSize(std::initializer_list<TValue> l) {
    assert(l.size() <= Size());
    Clear();
    iterator p = begin();
    for (auto v : l) *p++ = v;
  }

  constexpr TSelf& operator=(const TValue& v) {
    Fill(v);
    return *this;
  }

  constexpr TValue& operator()(unsigned i) { return data[i]; }

  constexpr const TValue& operator()(unsigned i) const { return data[i]; }

  constexpr iterator begin() { return &data.front(); }

  constexpr const_iterator begin() const { return &data.front(); }

  constexpr iterator end() { return begin() + size; }

  constexpr const_iterator end() const { return begin() + size; }

  constexpr iterator GetP(unsigned i) { return begin() + i; }

  constexpr const_iterator GetP(unsigned i) const { return begin() + i; }

  constexpr void swap(TSelf& r) { data.swap(r.data); }

  constexpr bool IsZero() const {
    constexpr TValue z = TValue();
    for (const_iterator p = begin(), pend = end(); p < pend;) {
      if (*p++ != z) return false;
    }
    return true;
  }

  constexpr bool operator==(const TSelf& r) const { return data == r.data; }

  constexpr bool operator!=(const TSelf& r) const { return data != r.data; }

  constexpr TSelf& operator+=(const TValue& v) {
    for (iterator p = begin(), pend = end(); p < pend;) *p++ += v;
    return *this;
  }

  constexpr TSelf& operator-=(const TValue& v) {
    for (iterator p = begin(), pend = end(); p < pend;) *p++ -= v;
    return *this;
  }

  constexpr TSelf& operator*=(const TValue& v) {
    for (iterator p = begin(), pend = end(); p < pend;) *p++ *= v;
    return *this;
  }

  constexpr TSelf& operator/=(const TValue& v) {
    for (iterator p = begin(), pend = end(); p < pend;) *p++ /= v;
    return *this;
  }

  constexpr TSelf operator+(const TValue& v) const {
    TSelf t(*this);
    t += v;
    return t;
  }

  constexpr TSelf operator-(const TValue& v) const {
    TSelf t(*this);
    t -= v;
    return t;
  }

  constexpr TSelf operator*(const TValue& v) const {
    TSelf t(*this);
    t *= v;
    return t;
  }

  constexpr TSelf operator/(const TValue& v) const {
    TSelf t(*this);
    t /= v;
    return t;
  }

  constexpr TSelf operator-() const {
    TSelf t(*this);
    for (iterator p = begin(), pend = end(); p < pend; ++p) *p = -*p;
    return t;
  }

  constexpr TSelf& operator+=(const TSelf& v) {
    const_iterator pv = v.begin();
    for (iterator p = begin(), pend = end(); p < pend;) *p++ += *pv++;
    return *this;
  }

  constexpr TSelf& operator-=(const TSelf& v) {
    const_iterator pv = v.begin();
    for (iterator p = begin(), pend = end(); p < pend;) *p++ -= *pv++;
    return *this;
  }

  constexpr TSelf operator+(const TSelf& v) const {
    TSelf t(*this);
    t += v;
    return t;
  }

  constexpr TSelf operator-(const TSelf& v) const {
    TSelf t(*this);
    t -= v;
    return t;
  }

  constexpr TValue DotProduct(const TSelf& v) const {
    TValue s = TValue();
    for (const_iterator p1 = begin(), p2 = v.begin(), pend = end(); p1 != pend;)
      s += *p1++ * *p2++;
    return s;
  }
};
}  // namespace la

template <unsigned size>
using DVectorStaticSize = la::VectorStaticSize<double, size>;
