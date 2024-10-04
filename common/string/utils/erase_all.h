#pragma once

#include <algorithm>
#include <string>

/**
 * @brief Removes all occurrences of a character from a string.
 *
 * This function takes a string and a character, and returns a new string
 * with all occurrences of the specified character removed.
 *
 * @param s The input string.
 * @param c The character to remove from the string.
 * @return A new string with all occurrences of the specified character removed.
 */
constexpr std::string EraseAll(const std::string& s, char c) {
  std::string result(s);
  result.erase(std::remove(result.begin(), result.end(), c), result.end());
  return result;
}
