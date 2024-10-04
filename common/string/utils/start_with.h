#pragma once

#include <string>

/**
 * @brief Checks if a string starts with a given prefix.
 *
 * This function checks if the input string starts with the specified prefix.
 *
 * @param str The input string to check.
 * @param prefix The prefix to check for.
 * @return true if the input string starts with the specified prefix, false
 * otherwise.
 */
constexpr bool StartWith(const std::string& str, const std::string& prefix) {
  if (str.size() < prefix.size()) {
    return false;
  }
  for (auto its = str.begin(), itp = prefix.begin(); itp != prefix.end();) {
    if (*its++ != *itp++) {
      return false;
    }
  }
  return true;
}
