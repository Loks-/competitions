#pragma once

#include <string>
#include <vector>

inline std::vector<std::string> Compress(const std::vector<std::string>& vs) {
  std::vector<std::string> vsnew;
  for (auto& s : vs) {
    if (!s.empty()) vsnew.push_back(s);
  }
  return vsnew;
}
