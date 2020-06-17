#pragma once

#include <string>

inline bool StartWith(const std::string& str, const std::string& prefix) {
  if (str.size() < prefix.size()) return false;
  for (auto its = str.begin(), itp = prefix.begin(); itp != prefix.end();) {
    if (*its++ != *itp++) return false;
  }
  return true;
}
