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
  void Fill(const TValue& v) { std::fill(data.begin(), data.end(), v); }
  void Clear() { Fill(TValue(0)); }
  unsigned Size() const { return unsigned(data.size()); }
  const TData& GetData() const { return data; }

  Vector(unsigned size) : data(size) {}
  Vector(unsigned size, const TValue& v) : data(size, v) {}
  Vector(const TData& v) : data(v) {}
  TSelf& operator=(const TValue& v) {
    Fill(v);
    return *this;
  }

  TValue& operator()(unsigned i) { return data[i]; }
  const TValue& operator()(unsigned i) const { return data[i]; }
  iterator begin() { return &data.front(); }
  const_iterator begin() const { return &data.front(); }
  iterator end() { return begin() + Size(); }
  const_iterator end() const { return begin() + Size(); }
  iterator GetP(unsigned i) { return begin() + i; }
  const_iterator GetP(unsigned i) const { return begin() + i; }
  void swap(TSelf& r) { data.swap(r.data); }

  TSelf& operator+=(const TValue& v) {
    for (iterator p = begin(), pend = end(); p < pend;) *p++ += v;
    return *this;
  }

  TSelf& operator-=(const TValue& v) {
    for (iterator p = begin(), pend = end(); p < pend;) *p++ -= v;
    return *this;
  }

  TSelf& operator*=(const TValue& v) {
    for (iterator p = begin(), pend = end(); p < pend;) *p++ *= v;
    return *this;
  }

  TSelf& operator/=(const TValue& v) {
    for (iterator p = begin(), pend = end(); p < pend;) *p++ /= v;
    return *this;
  }

  TSelf operator+(const TValue& v) const {
    TSelf t(*this);
    t += v;
    return t;
  }

  TSelf operator-(const TValue& v) const {
    TSelf t(*this);
    t -= v;
    return t;
  }

  TSelf operator*(const TValue& v) const {
    TSelf t(*this);
    t *= v;
    return t;
  }

  TSelf operator/(const TValue& v) const {
    TSelf t(*this);
    t /= v;
    return t;
  }

  TSelf operator-() const {
    TSelf t(*this);
    for (iterator p = t.begin(), pend = t.end(); p < pend; ++p) *p = -*p;
    return t;
  }

  TSelf& operator+=(const TSelf& v) {
    assert(Size() == v.Size());
    const_iterator pv = v.begin();
    for (iterator p = begin(), pend = end(); p < pend;) *p++ += *pv++;
    return *this;
  }

  TSelf& operator-=(const TSelf& v) {
    assert(Size() == v.Size());
    const_iterator pv = v.begin();
    for (iterator p = begin(), pend = end(); p < pend;) *p++ -= *pv++;
    return *this;
  }

  TSelf operator+(const TSelf& v) const {
    TSelf t(*this);
    t += v;
    return t;
  }

  TSelf operator-(const TSelf& v) const {
    TSelf t(*this);
    t -= v;
    return t;
  }
};
}  // namespace la

using DVector = la::Vector<double>;
