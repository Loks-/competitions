#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace la {
template <class TTValue>
class Vector {
 public:
  using TValue = TTValue;
  using TData = std::vector<TValue>;
  using TSelf = Vector<TValue>;

  using iterator = TValue*;
  using const_iterator = const TValue*;

 protected:
  TData data;

 public:
  constexpr void Fill(const TValue& v) {
    std::fill(data.begin(), data.end(), v);
  }

  constexpr void Clear() { Fill(TValue(0)); }

  constexpr unsigned Size() const { return unsigned(data.size()); }

  constexpr const TData& GetData() const { return data; }

  constexpr explicit Vector(unsigned size) : data(size) {}

  constexpr Vector(unsigned size, const TValue& v) : data(size, v) {}

  constexpr Vector(const TData& v) : data(v) {}

  constexpr TSelf& operator=(const TValue& v) {
    Fill(v);
    return *this;
  }

  constexpr TValue& operator()(unsigned i) { return data[i]; }

  constexpr const TValue& operator()(unsigned i) const { return data[i]; }

  constexpr iterator begin() { return &data.front(); }

  constexpr const_iterator begin() const { return &data.front(); }

  constexpr iterator end() { return begin() + Size(); }

  constexpr const_iterator end() const { return begin() + Size(); }

  constexpr iterator GetP(unsigned i) { return begin() + i; }

  constexpr const_iterator GetP(unsigned i) const { return begin() + i; }

  constexpr void swap(TSelf& r) { data.swap(r.data); }

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
    for (iterator p = t.begin(), pend = t.end(); p < pend; ++p) *p = -*p;
    return t;
  }

  constexpr TSelf& operator+=(const TSelf& v) {
    assert(Size() == v.Size());
    const_iterator pv = v.begin();
    for (iterator p = begin(), pend = end(); p < pend;) *p++ += *pv++;
    return *this;
  }

  constexpr TSelf& operator-=(const TSelf& v) {
    assert(Size() == v.Size());
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
    assert(Size() == v.Size());
    TValue s = TValue();
    for (const_iterator p1 = begin(), p2 = v.begin(), pend = end(); p1 != pend;)
      s += *p1++ * *p2++;
    return s;
  }
};
}  // namespace la

using DVector = la::Vector<double>;
