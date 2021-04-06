#pragma once

#include <iostream>
#include <vector>

namespace nvector {
template <class T>
inline std::vector<T> Read(unsigned N) {
  std::vector<T> v(N);
  for (unsigned i = 0; i < N; ++i) std::cin >> v[i];
  return v;
}
}  // namespace nvector
