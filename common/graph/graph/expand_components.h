#pragma once

#include "common/vector/max.h"

#include <vector>

inline std::vector<std::vector<unsigned>> ExpandComponents(
    const std::vector<unsigned>& vc) {
  std::vector<std::vector<unsigned>> vvc(nvector::Max(vc) + 1);
  for (unsigned i = 0; i < vc.size(); ++i) {
    vvc[vc[i]].push_back(i);
  }
  return vvc;
}
