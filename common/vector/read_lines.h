#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace nvector {
inline std::vector<std::string> ReadLines() {
  std::vector<std::string> vs;
  std::string line;
  while (std::getline(std::cin, line)) vs.push_back(line);
  return vs;
}
}  // namespace nvector
