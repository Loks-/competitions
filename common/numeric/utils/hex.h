#pragma once

#include "common/base.h"

#include <string>

inline unsigned HexToU(char c) {
  return std::isdigit(c) ? (c - '0') : (c - 'A' + 10);
}

inline unsigned HexToU32(const std::string& s) {
  unsigned x = 0;
  for (auto c : s) x = x * 16 + HexToU(c);
  return x;
}

inline uint64_t HexToU64(const std::string& s) {
  uint64_t x = 0;
  for (auto c : s) x = x * 16 + HexToU(c);
  return x;
}
