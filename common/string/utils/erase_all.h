#pragma once

#include <algorithm>
#include <string>

constexpr std::string EraseAll(const std::string& s, char c) {
  std::string t(s);
  t.erase(std::remove(t.begin(), t.end(), c), t.end());
  return t;
}
