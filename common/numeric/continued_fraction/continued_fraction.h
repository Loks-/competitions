#pragma once

#include "common/base.h"
#include "common/numeric/fraction.h"

#include <algorithm>
#include <vector>

namespace numeric {
namespace cf {
class ContinuedFraction {
 public:
  using TCFVector = std::vector<int64_t>;

 protected:
  std::vector<int64_t> cf;

 public:
  constexpr explicit ContinuedFraction(
      const std::vector<int64_t>& continued_fraction)
      : cf(continued_fraction) {
    if (cf.size() == 0) cf.push_back(0);
  }

  constexpr explicit ContinuedFraction(const TIFraction& f) {
    int64_t n = f.GetN(), d = f.GetD();
    if (d == 0) {
      assert(n > 0);
      return;
    }
    int64_t a = n / d;
    if (a * d > n) --a;
    cf.push_back(a);
    n -= a * d;
    for (; n;) {
      std::swap(n, d);
      a = n / d;
      cf.push_back(a);
      n -= a * d;
    }
  }

  constexpr const std::vector<int64_t>& GetVector() const { return cf; }

  constexpr unsigned Size() const { return unsigned(cf.size()); }

  constexpr int64_t operator()(unsigned index) const { return cf[index]; }

  constexpr void SplitLast() {
    if (!cf.empty() && ((cf.back() > 1) || (cf.size() == 1))) {
      cf.back() -= 1;
      cf.push_back(1);
    }
  }

  constexpr TIFraction ToFraction() const {
    int64_t n0 = 1, n1 = 0, d0 = 0, d1 = 1;
    for (int64_t a : cf) {
      int64_t nn = a * n0 + n1;
      n1 = n0;
      n0 = nn;
      int64_t dn = a * d0 + d1;
      d1 = d0;
      d0 = dn;
    }
    return TIFraction(n0, d0);
  }

  constexpr bool operator==(const ContinuedFraction& r) const {
    return cf == r.cf;
  }

  constexpr bool operator<(const ContinuedFraction& r) const {
    const unsigned ls = Size(), rs = r.Size(), s = std::min(ls, rs);
    for (unsigned i = 0; i < s; ++i) {
      const int64_t d = cf[i] - r.cf[i];
      if (d) return ((i & 1) == 0) == (d < 0);
    }
    if (ls == rs) return false;
    return ((s & 1) == 0) != (ls == s);
  }
};
}  // namespace cf
}  // namespace numeric
