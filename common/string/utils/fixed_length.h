#pragma once

#include <string>

inline std::string FixedLength(const std::string& input, unsigned length,
                               char c = ' ') {
  std::string s(input);
  s.resize(length, c);
  return s;
}
