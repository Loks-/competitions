#pragma once

#include <string>
#include <vector>

/**
 * @brief Converts a string to a vector of characters.
 *
 * This function takes an input string and converts it to a vector of
 * characters.
 *
 * @param s The input string to convert.
 * @return A vector of characters representing the input string.
 */
constexpr std::vector<char> ToVector(const std::string& s) {
  return std::vector<char>(s.begin(), s.end());
}
