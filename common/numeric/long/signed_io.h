#pragma once

#include "common/numeric/long/signed.h"
#include "common/numeric/long/unsigned_io.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>

namespace numeric {
namespace nlong {
constexpr Signed SignedParse(const std::string& s, unsigned base = 10) {
  return s.empty()       ? Signed()
         : (s[0] == '-') ? Signed(false, UnsignedParse(s.substr(1), base))
                         : Signed(UnsignedParse(s, base));
}

inline std::istream& operator>>(std::istream& s, Signed& ls) {
  std::string str;
  s >> str;
  ls = SignedParse(str);
  return s;
}

inline Signed SignedReadBase(unsigned base = 10) {
  std::string s;
  std::cin >> s;
  return SignedParse(s, base);
}

constexpr std::string ToString(const Signed& ls, unsigned base = 10) {
  return (ls.Sign() ? "" : "-") + ToString(ls.GetUnsigned(), base);
}

inline std::ostream& operator<<(std::ostream& s, const Signed& ls) {
  return s << ToString(ls);
}

inline void Write(const Signed& ls, bool add_eod = true, unsigned base = 10) {
  std::string s = ToString(ls, base);
  std::cout << s;
  if (add_eod) std::cout << std::endl;
}
}  // namespace nlong
}  // namespace numeric
