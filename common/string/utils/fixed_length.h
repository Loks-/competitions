#pragma once

#include <string>

/**
 * @brief Adjusts the length of a string to a fixed length.
 *
 * This function takes an input string and adjusts its length to the specified
 * length. If the input string is shorter than the specified length, it is
 * padded with the specified character. If the input string is longer than the
 * specified length, it is truncated.
 *
 * @param input The input string to adjust.
 * @param length The desired length of the output string.
 * @param c The character to use for padding if the input string is shorter than
 * the specified length. Default is space (' ').
 * @return A string adjusted to the specified length.
 */
constexpr std::string FixedLength(const std::string& input, unsigned length,
                                  char c = ' ') {
  std::string s(input);
  s.resize(length, c);
  return s;
}
