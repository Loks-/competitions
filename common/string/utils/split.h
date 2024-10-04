#pragma once

#include "common/string/utils/compress.h"

#include <string>
#include <unordered_set>
#include <vector>

/**
 * @brief Splits a string by a single delimiter, ignoring empty segments.
 *
 * This function splits the input string by the specified delimiter and returns
 * a vector of non-empty substrings.
 *
 * @param s The input string to split.
 * @param delimiter The character to split the string by.
 * @return A vector of non-empty substrings.
 */
constexpr std::vector<std::string> Split(const std::string& s, char delimiter) {
  if (s.empty()) return {};
  std::vector<std::string> vs;
  size_t npos = 0;
  while (true) {
    size_t npos_next = s.find(delimiter, npos);
    if (npos_next == std::string::npos) break;
    if (npos_next != npos) {
      vs.emplace_back(s.substr(npos, npos_next - npos));
    }
    npos = npos_next + 1;
  }
  if (npos < s.size()) {
    vs.emplace_back(s.substr(npos));
  }
  return vs;
}

/**
 * @brief Splits a string by a single delimiter, including empty segments.
 *
 * This function splits the input string by the specified delimiter and returns
 * a vector of substrings, including empty substrings.
 *
 * @param s The input string to split.
 * @param delimiter The character to split the string by.
 * @return A vector of substrings, including empty substrings.
 */
constexpr std::vector<std::string> SplitAll(const std::string& s,
                                            char delimiter) {
  if (s.empty()) return {};
  std::vector<std::string> vs;
  size_t npos = 0;
  while (true) {
    size_t npos_next = s.find(delimiter, npos);
    if (npos_next == std::string::npos) break;
    vs.emplace_back(s.substr(npos, npos_next - npos));
    npos = npos_next + 1;
  }
  vs.emplace_back(s.substr(npos));
  return vs;
}

/**
 * @brief Splits a string by multiple delimiters, including empty segments.
 *
 * This function splits the input string by any of the specified delimiters and
 * returns a vector of substrings, including empty substrings.
 *
 * @param s The input string to split.
 * @param delimiters A string containing all delimiter characters.
 * @return A vector of substrings, including empty substrings.
 */
inline std::vector<std::string> SplitAll(const std::string& s,
                                         const std::string& delimiters) {
  std::vector<std::string> vs;
  const std::unordered_set<char> us(delimiters.begin(), delimiters.end());
  std::string scur;
  for (auto c : s) {
    if (us.find(c) == us.end()) {
      scur.push_back(c);
    } else {
      vs.push_back(scur);
      scur.clear();
    }
  }
  vs.push_back(scur);
  return vs;
}

/**
 * @brief Splits a string by multiple delimiters, ignoring empty segments.
 *
 * This function splits the input string by any of the specified delimiters and
 * returns a vector of non-empty substrings.
 *
 * @param s The input string to split.
 * @param delimiters A string containing all delimiter characters.
 * @return A vector of non-empty substrings.
 */
inline std::vector<std::string> Split(const std::string& s,
                                      const std::string& delimiters) {
  return Compress(SplitAll(s, delimiters));
}
