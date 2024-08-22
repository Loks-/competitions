#pragma once

#include "common/base.h"

namespace la {
template <class TVector>
constexpr bool AVLess(const TVector& l, const TVector& r) {
  assert(l.Size() == r.Size());
  for (auto pl = l.begin(), pr = r.begin(), pend = l.end(); pl < pend;) {
    if (*pl++ >= *pr++) return false;
  }
  return true;
}

template <class TVector>
constexpr bool AVLessOrEqual(const TVector& l, const TVector& r) {
  assert(l.Size() == r.Size());
  for (auto pl = l.begin(), pr = r.begin(), pend = l.end(); pl < pend;) {
    if (*pl++ > *pr++) return false;
  }
  return true;
}
}  // namespace la
