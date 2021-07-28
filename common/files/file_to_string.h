#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace files {
inline std::string FileToString(const std::string& filename) {
  std::ifstream f(filename);
  if (!f.is_open()) return "";
  std::stringstream buffer;
  buffer << f.rdbuf();
  return buffer.str();
}
}  // namespace files
