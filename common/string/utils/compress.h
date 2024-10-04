#pragma once

#include <string>
#include <vector>

/**
 * @brief Removes empty strings from a vector of strings.
 *
 * This function takes a vector of strings and returns a new vector
 * containing only the non-empty strings from the input vector.
 *
 * @param vs The input vector of strings.
 * @return A new vector containing only the non-empty strings from the input
 * vector.
 */
constexpr std::vector<std::string> Compress(
    const std::vector<std::string>& vs) {
  std::vector<std::string> vsnew;
  vsnew.reserve(vs.size());  // Reserve space to avoid multiple allocations
  for (const auto& s : vs) {
    if (!s.empty()) {
      vsnew.push_back(s);
    }
  }
  return vsnew;
}
