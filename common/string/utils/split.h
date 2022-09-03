#pragma once

#include "common/string/utils/compress.h"

#include <string>
#include <unordered_set>
#include <vector>

inline std::vector<std::string> Split(const std::string& s, char delimeter) {
  if (s.empty()) return {};
  std::vector<std::string> vs;
  size_t npos = 0;
  for (size_t npos_next = s.find(delimeter, npos);
       npos_next != std::string::npos; npos_next = s.find(delimeter, npos)) {
    vs.emplace_back(s.substr(npos, npos_next - npos));
    npos = npos_next + 1;
  }
  vs.emplace_back(s.substr(npos));
  return vs;
}

inline std::vector<std::string> SplitAll(const std::string& s,
                                         const std::string& delimeters) {
  std::vector<std::string> vs;
  std::unordered_set<char> us(delimeters.begin(), delimeters.end());
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

inline std::vector<std::string> Split(const std::string& s,
                                      const std::string& delimeters) {
  return Compress(SplitAll(s, delimeters));
}
