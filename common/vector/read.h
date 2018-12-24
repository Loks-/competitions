#pragma once

#include <iostream>
#include <vector>

template <class T>
inline std::vector<T> ReadVector(unsigned N) {
  std::vector<T> v(N);
  for (unsigned i = 0; i < N; ++i) std::cin >> v[i];
  return v;
}
