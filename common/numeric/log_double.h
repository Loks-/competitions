#pragma once

#include "common/base.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace numeric {
class LogDouble {
 protected:
  constexpr static double log0 = -std::numeric_limits<double>::infinity();
  double value;

 public:
  constexpr static double safe_log(double x) {
    assert(x >= 0.);
    return (x == 0.) ? log0 : log(x);
  }

  constexpr LogDouble() : value(log0) {}

  constexpr LogDouble(double x) { value = safe_log(x); }

  constexpr double Get() const { return exp(value); }

  constexpr double GetLog() const { return value; }

  constexpr void SetLog(double log_x) { value = log_x; }

  constexpr static LogDouble MakeLog(double log_x) {
    LogDouble t;
    t.SetLog(log_x);
    return t;
  }

  constexpr bool operator<(const LogDouble& r) const { return value < r.value; }

  constexpr bool operator<=(const LogDouble& r) const {
    return !(r.value < value);
  }

  constexpr LogDouble& operator+=(const LogDouble& r) {
    *this = (*this + r);
    return *this;
  }

  constexpr LogDouble& operator-=(const LogDouble& r) {
    *this = (*this - r);
    return *this;
  }

  constexpr LogDouble& operator*=(const LogDouble& r) {
    value += r.value;
    return *this;
  }

  constexpr LogDouble& operator/=(const LogDouble& r) {
    value -= r.value;
    return *this;
  }

  constexpr LogDouble operator+(const LogDouble& r) const {
    if (value == log0) return r;
    if (r.value == log0) return *this;
    const double v1 = std::max(value, r.value);
    const double v2 = std::min(value, r.value);
    return MakeLog(v1 + log(1.0 + exp(v2 - v1)));
  }

  constexpr LogDouble operator-(const LogDouble& r) const {
    if (r.value == log0) return *this;
    if (value == r.value) return LogDouble();
    assert(r.value < value);
    return MakeLog(value + log(1.0 - exp(r.value - value)));
  }

  constexpr LogDouble operator*(const LogDouble& r) const {
    LogDouble t(*this);
    t *= r;
    return t;
  }

  constexpr LogDouble operator/(const LogDouble& r) const {
    LogDouble t(*this);
    t /= r;
    return t;
  }
};
}  // namespace numeric
