#pragma once

#include "common/numeric/long/unsigned.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>

namespace numeric {
namespace nlong {
constexpr Unsigned UnsignedParse(const std::string& s, unsigned base = 10) {
  assert(base == 10);
  Unsigned lu;
  for (char c : s) {
    lu *= base;
    assert((c >= '0') && (c <= '9'));
    lu += unsigned(c - '0');
  }
  return lu;
}

inline std::istream& operator>>(std::istream& s, Unsigned& lu) {
  std::string str;
  s >> str;
  lu = UnsignedParse(str);
  return s;
}

inline Unsigned UnsignedReadBase(unsigned base = 10) {
  std::string s;
  std::cin >> s;
  return UnsignedParse(s, base);
}

constexpr std::string ToString(const Unsigned& lu, unsigned base = 10) {
  if (lu.Empty()) return "0";
  assert(base == 10);
  std::vector<unsigned> v = lu.ToVector(base);
  std::reverse(v.begin(), v.end());
  std::string s;
  s.reserve(v.size());
  for (unsigned u : v) s.push_back('0' + char(u));
  return s;
}

inline std::ostream& operator<<(std::ostream& s, const Unsigned& lu) {
  return s << ToString(lu);
}

inline void Write(const Unsigned& lu, bool add_eod = true, unsigned base = 10) {
  const std::string s = ToString(lu, base);
  std::cout << s;
  if (add_eod) std::cout << std::endl;
}
}  // namespace nlong
}  // namespace numeric
