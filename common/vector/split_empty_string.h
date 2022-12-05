#pragma once

#include <string>
#include <vector>

namespace nvector {
inline std::vector<std::vector<std::string>> SplitEmptyString(
    const std::vector<std::string>& vs) {
  std::vector<std::vector<std::string>> vvs(vs.empty() ? 0 : 1);
  for (auto& s : vs) {
    if (s.empty()) {
      vvs.push_back({});
    } else {
      vvs.back().push_back(s);
    }
  }
  return vvs;
}
}  // namespace nvector
