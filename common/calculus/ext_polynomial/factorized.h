#pragma once

#include <vector>

namespace calculus {
namespace ext_polynomial {
// (x_i - c)
template <class TValue>
class FactorizedLinear {
 public:
  unsigned index;
  TValue c;

  FactorizedLinear(unsigned _index, const TValue& _c = TValue(0))
      : index(_index), c(_c) {}
};

template <class TValue>
class Factorized {
 public:
  using TLinear = FactorizedLinear<TValue>;
  using TSelf = Factorized<TValue>;

  TValue a;
  std::vector<TLinear> vn, vd;

  Factorized() : a(1) {}
  Factorized(const TLinear& l, const TValue& _a = TValue(1))
      : a(_a), vn(1, l) {}

  bool SimpleD() const {
    for (auto& l : vd) {
      if (l.c != TValue(0)) return false;
    }
    return true;
  }

  TSelf& operator*=(const TValue& r) {
    a *= r;
    return *this;
  }

  TSelf& operator*=(const TSelf& r) {
    a *= r.a;
    vn.insert(vn.end(), r.vn.begin(), r.vn.end());
    vd.insert(vd.end(), r.vd.begin(), r.vd.end());
    return *this;
  }

  TSelf& operator/=(const TValue& r) {
    a *= r;
    return *this;
  }

  TSelf& operator/=(const TSelf& r) {
    a /= r.a;
    vn.insert(vn.end(), r.vd.begin(), r.vd.end());
    vd.insert(vd.end(), r.vn.begin(), r.vn.end());
    return *this;
  }

  TSelf operator*(const TValue& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  TSelf operator*(const TSelf& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  TSelf operator/(const TValue& r) const {
    TSelf t(*this);
    t /= r;
    return t;
  }

  TSelf operator/(const TSelf& r) const {
    TSelf t(*this);
    t /= r;
    return t;
  }

  TSelf operator-() const { return *this * (-TValue(1)); }
};

using DFactorized = Factorized<double>;

template <class TValue>
inline Factorized<TValue> MakeFXi(unsigned i, const TValue& c = TValue(0),
                                  const TValue& a = TValue(1)) {
  return Factorized<TValue>(FactorizedLinear<TValue>(i, c), a);
}

inline DFactorized DMakeFXi(unsigned i, double c = 0., double a = 1.) {
  return MakeFXi<double>(i, c, a);
}
}  // namespace ext_polynomial
}  // namespace calculus
