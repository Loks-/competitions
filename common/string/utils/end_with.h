#pragma once

#include <string>

/**
 * @brief Checks if a string ends with a given suffix.
 *
 * This function checks if the input string ends with the specified suffix.
 *
 * @param str The input string to check.
 * @param suffix The suffix to check for.
 * @return true if the input string ends with the specified suffix, false
 * otherwise.
 */
constexpr bool EndWith(const std::string& str, const std::string& suffix) {
  if (str.size() < suffix.size()) {
    return false;
  }
  for (auto its = str.rbegin(), itp = suffix.rbegin(); itp != suffix.rend();) {
    if (*its++ != *itp++) {
      return false;
    }
  }
  return true;
}
