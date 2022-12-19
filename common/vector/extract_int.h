#pragma once

#include "common/string/utils/split.h"

#include <string>
#include <vector>

namespace nvector {
template <class T>
inline std::vector<T> ExtractInt(const std::string& s,
                                 const std::string& delimeters = " ") {
  std::vector<T> v;
  auto ss = Split(s, delimeters);
  for (const auto& si : ss) {
    char* p = nullptr;
    long ltemp = strtol(si.c_str(), &p, 10);
    if (!*p) v.push_back(ltemp);
  }
  return v;
}
}  // namespace nvector
