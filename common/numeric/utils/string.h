#pragma once

#include <algorithm>
#include <string>

namespace numeric {
template <class TValue>
constexpr std::string UValueToString(const TValue& u, unsigned base = 10) {
  assert(base <= 10);
  if (u == 0) return "0";
  std::string s;
  for (auto uu = u; uu; uu /= base) {
    s.push_back('0' + char(uu % TValue(base)));
  }
  std::reverse(s.begin(), s.end());
  return s;
}
}  // namespace numeric
