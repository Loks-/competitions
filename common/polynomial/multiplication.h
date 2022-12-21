#pragma once

#include "common/polynomial/polynomial.h"

namespace polynomial {
template <class TValue>
inline Polynomial<TValue> MultBase(const Polynomial<TValue>& a,
                                   const Polynomial<TValue>& b) {
  Polynomial<TValue> r;
  for (auto it = b.end(); it != b.begin();) {
    r.MultX();
    r += a * *(--it);
  }
  return r;
}

template <class TValue>
inline Polynomial<TValue> operator*(const Polynomial<TValue>& l,
                                    const Polynomial<TValue>& r) {
  return MultBase(l, r);
}
}  // namespace polynomial
