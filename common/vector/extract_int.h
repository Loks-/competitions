#pragma once

#include "common/string/utils/split.h"

#include <string>
#include <vector>

namespace nvector {

/**
 * @brief Extracts integers from a string based on specified delimiters.
 *
 * This function splits the input string `s` using the specified delimiters,
 * converts the resulting substrings to integers, and returns them in a vector.
 *
 * @tparam T The type of the integers to be extracted.
 * @param s The input string from which to extract integers.
 * @param delimiters The delimiters used to split the input string. Default is a
 * space.
 * @return A vector of integers extracted from the input string.
 */
template <class T>
inline std::vector<T> ExtractInt(const std::string& s,
                                 const std::string& delimeters = " ") {
  std::vector<T> v;
  auto ss = Split(s, delimeters);
  for (const auto& si : ss) {
    char* p = nullptr;
    long ltemp = strtol(si.c_str(), &p, 10);
    if (!*p) {
      v.push_back(static_cast<T>(ltemp));
    }
  }
  return v;
}

}  // namespace nvector
