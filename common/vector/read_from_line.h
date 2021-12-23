#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "common/vector/read_from_string.h"

namespace nvector {
template <class T>
inline std::vector<T> ReadFromLine(char delimeter) {
  std::string line;
  if (!std::getline(std::cin, line)) return {};
  return ReadFromString<T>(line, delimeter);
}

template <class T>
inline std::vector<T> ReadFromLine(const std::string& delimeters) {
  std::string line;
  if (!std::getline(std::cin, line)) return {};
  return ReadFromString<T>(line, delimeters);
}
}  // namespace nvector
