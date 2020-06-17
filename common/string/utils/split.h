#pragma once

#include <string>
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
