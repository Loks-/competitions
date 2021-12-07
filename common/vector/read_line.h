#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "common/string/utils/split.h"

namespace nvector {
template <class T>
inline std::vector<T> ReadLine(char delimeter) {
  std::string line;
  if (!std::getline(std::cin, line)) return {};
  auto ls = Split(line, delimeter);
  std::vector<T> v(ls.size());
  for (unsigned i = 0; i < ls.size(); ++i) std::istringstream(ls[i]) >> v[i];
  return v;
}
}  // namespace nvector
