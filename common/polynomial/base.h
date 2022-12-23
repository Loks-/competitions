#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace polynomial {
template <class TTValue, class TTMe>
class Base {
 public:
  using TValue = TTValue;
  using TData = std::vector<TValue>;
  using TMe = TTMe;
  using TSelf = Base<TValue, TMe>;

  using iterator = TValue*;
  using const_iterator = const TValue*;

 protected:
  TData data;

 public:
  TMe* Me() { return static_cast<TMe*>(this); }
  const TMe* Me() const { return static_cast<const TMe*>(this); }

  void Clear() { data.clear(); }
  bool Empty() const { return data.size() == 0; }
  unsigned Size() const { return unsigned(data.size()); }
  unsigned Power() const { return std::max<unsigned>(Size(), 1) - 1; }

  void Normalize() {
    for (; (data.size() > 0) && (data.back() == TValue());) data.pop_back();
  }

  TValue GetCoefficient(unsigned i) const {
    return (i < data.size()) ? data[i] : TValue();
  }

  TValue operator[](unsigned i) const { return GetCoefficient(i); }

  void SetCoefficient(unsigned i, const TValue& new_value) {
    if (data.size() <= i) data.resize(i + 1);
    data[i] = new_value;
    Normalize();
  }

  iterator begin() { return &data.front(); }
  const_iterator begin() const { return &data.front(); }
  iterator end() { return begin() + Size(); }
  const_iterator end() const { return begin() + Size(); }
  void swap(TSelf& r) { data.swap(r.data); }

  TMe& operator+=(const TValue& v) {
    if (Empty())
      data.push_back(v);
    else
      data[0] += v;
    return *Me();
  }

  TMe& operator-=(const TValue& v) {
    if (Empty())
      data.push_back(-v);
    else
      data[0] -= v;
    return *Me();
  }

  TMe& operator*=(const TValue& v) {
    for (TValue *p = begin(), *pend = end(); p < pend;) *p++ *= v;
    Normalize();
    return *Me();
  }

  TMe& operator/=(const TValue& v) {
    assert(v != TValue());
    for (TValue *p = begin(), *pend = end(); p < pend;) *p++ /= v;
    return *Me();
  }

  TMe operator+(const TValue& v) const {
    TMe t(*Me());
    t += v;
    return t;
  }

  TMe operator-(const TValue& v) const {
    TMe t(*Me());
    t -= v;
    return t;
  }

  TMe operator*(const TValue& v) const {
    TMe t(*Me());
    t *= v;
    return t;
  }

  TMe operator/(const TValue& v) const {
    TMe t(*Me());
    t /= v;
    return t;
  }

  TMe operator-() const {
    TMe t(*Me());
    for (auto p = t.begin(), pend = t.end(); p < pend; ++p) *p = -*p;
    return t;
  }

  TMe& operator+=(const TMe& v) {
    if (Size() < v.Size()) data.resize(v.Size());
    auto p = begin();
    for (auto pv = v.begin(), pvend = v.end(); pv < pvend;) *p++ += *pv++;
    Normalize();
    return *Me();
  }

  TMe& operator-=(const TMe& v) {
    if (Size() < v.Size()) data.resize(v.Size());
    auto p = begin();
    for (auto pv = v.begin(), pvend = v.end(); pv < pvend;) *p++ -= *pv++;
    Normalize();
    return *Me();
  }

  TMe operator+(const TMe& v) const {
    TMe t(*Me());
    t += v;
    return t;
  }

  TMe operator-(const TMe& v) const {
    TMe t(*Me());
    t -= v;
    return t;
  }
};
}  // namespace polynomial
