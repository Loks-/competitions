#pragma once

#include "common/base.h"
#include <algorithm>
#include <cmath>
#include <limits>

class LogDouble {
 protected:
  constexpr static double log0 = -std::numeric_limits<double>::infinity();
  double value;

 public:
  static double safe_log(double x) {
    assert(x >= 0.);
    return (x == 0.) ? log0 : log(x);
  }

  LogDouble() : value(log0) {}
  LogDouble(double x) { value = safe_log(x); }

  double Get() const { return exp(value); }
  double GetLog() const { return value; }
  void SetLog(double log_x) { value = log_x; }

  static LogDouble MakeLog(double log_x) {
    LogDouble t;
    t.SetLog(log_x);
    return t;
  }

  bool operator<(const LogDouble& r) const { return value < r.value; }
  bool operator<=(const LogDouble& r) const { return !(r.value < value); }

  LogDouble& operator+=(const LogDouble& r) {
    *this = (*this + r);
    return *this;
  }

  LogDouble& operator-=(const LogDouble& r) {
    *this = (*this - r);
    return *this;
  }

  LogDouble& operator*=(const LogDouble& r) {
    value += r.value;
    return *this;
  }

  LogDouble& operator/=(const LogDouble& r) {
    value -= r.value;
    return *this;
  }

  LogDouble operator+(const LogDouble& r) const {
    if (value == log0) return r;
    if (r.value == log0) return *this;
    double v1 = std::max(value, r.value);
    double v2 = std::min(value, r.value);
    return MakeLog(v1 + log(1.0 + exp(v2 - v1)));
  }

  LogDouble operator-(const LogDouble& r) const {
    if (r.value == log0) return *this;
    if (value == r.value) return LogDouble();
    assert(r.value < value);
    return MakeLog(value + log(1.0 - exp(r.value - value)));
  }

  LogDouble operator*(const LogDouble& r) const {
    LogDouble t(*this);
    t *= r;
    return t;
  }

  LogDouble operator/(const LogDouble& r) const {
    LogDouble t(*this);
    t /= r;
    return t;
  }
};
