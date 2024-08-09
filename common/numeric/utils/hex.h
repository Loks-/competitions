#pragma once

#include "common/base.h"
#include "common/string/utils/is_digit.h"

#include <string>

constexpr unsigned HexToU(char c) {
  return IsDigit(c) ? (c - '0') : (c - 'A' + 10);
}

constexpr unsigned HexToU32(const std::string& s) {
  unsigned x = 0;
  for (auto c : s) x = x * 16 + HexToU(c);
  return x;
}

constexpr uint64_t HexToU64(const std::string& s) {
  uint64_t x = 0;
  for (auto c : s) x = x * 16 + HexToU(c);
  return x;
}
