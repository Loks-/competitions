#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "common/string/utils/split.h"

namespace nvector {
template <class T>
inline std::vector<T> ReadFromString(const std::string& s, char delimeter) {
  auto ls = Split(s, delimeter);
  std::vector<T> v(ls.size());
  for (unsigned i = 0; i < ls.size(); ++i) std::istringstream(ls[i]) >> v[i];
  return v;
}

template <class T>
inline std::vector<T> ReadFromString(const std::string& s,
                                     const std::string& delimeters) {
  auto ls = Split(s, delimeters);
  std::vector<T> v(ls.size());
  for (unsigned i = 0; i < ls.size(); ++i) std::istringstream(ls[i]) >> v[i];
  return v;
}
}  // namespace nvector
