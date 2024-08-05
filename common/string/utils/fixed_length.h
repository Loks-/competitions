#pragma once

#include <string>

constexpr std::string FixedLength(const std::string& input, unsigned length,
                                  char c = ' ') {
  std::string s(input);
  s.resize(length, c);
  return s;
}
