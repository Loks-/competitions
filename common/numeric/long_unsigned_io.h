#pragma once

#include "long_unsigned.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>

inline LongUnsigned LongUnsignedParse(const std::string& s,
                                      unsigned base = 10) {
  assert(base == 10);
  LongUnsigned lu;
  for (char c : s) {
    lu *= base;
    assert((c >= '0') && (c <= '9'));
    lu += unsigned(c - '0');
  }
  return lu;
}

inline std::istream& operator>>(std::istream& s, LongUnsigned& lu) {
  std::string str;
  s >> str;
  lu = LongUnsignedParse(str);
  return s;
}

inline LongUnsigned LongUnsignedReadBase(unsigned base = 10) {
  std::string s;
  std::cin >> s;
  return LongUnsignedParse(s, base);
}

inline std::string ToString(const LongUnsigned& lu, unsigned base = 10) {
  if (lu.Empty()) return "0";
  assert(base == 10);
  std::vector<unsigned> v = lu.ToVector(base);
  std::reverse(v.begin(), v.end());
  std::string s;
  s.reserve(v.size());
  for (unsigned u : v) s.push_back('0' + char(u));
  return s;
}

inline std::ostream& operator<<(std::ostream& s, const LongUnsigned& lu) {
  s << ToString(lu);
  return s;
}

inline void Write(const LongUnsigned& lu, bool add_eod = true,
                  unsigned base = 10) {
  std::string s = ToString(base);
  std::cout << s;
  if (add_eod) std::cout << std::endl;
}
