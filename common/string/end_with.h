#pragma once

#include <string>

inline bool EndWith(const std::string& str, const std::string& prefix) {
  if (str.size() < prefix.size()) return false;
  for (auto its = str.rbegin(), itp = prefix.rbegin(); itp != prefix.rend();) {
    if (*its++ != *itp++) return false;
  }
  return true;
}
