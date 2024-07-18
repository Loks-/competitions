#pragma once

#include <algorithm>
#include <random>
#include <vector>

namespace nvector {
template <class T>
void Shuffle(std::vector<T>& v) {
  std::shuffle(v.begin(), v.end(), std::default_random_engine(17));
}
}  // namespace nvector
