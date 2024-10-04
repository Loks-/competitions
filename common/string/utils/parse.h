#pragma once

#include "common/base.h"

#include <string>

/**
 * @brief Parses a string representing a number in a given base.
 *
 * This function converts a string representing a number in a specified base
 * to its numeric value of type T.
 *
 * @tparam T The type of the numeric value. Default is uint64_t.
 * @param s The input string representing the number.
 * @param base The base of the number in the string. Default is 10.
 * @return The numeric value of type T.
 */
template <class T = uint64_t>
constexpr T Parse(const std::string& s, T base = 10) {
  T x = 0;
  for (char c : s) {
    x *= base;
    if ((c >= '0') && (c <= '9')) {
      x += (c - '0');
    } else if ((c >= 'A') && (c <= 'Z')) {
      x += (c - 'A') + 10;
    } else {
      assert(false);
    }
  }
  return x;
}
