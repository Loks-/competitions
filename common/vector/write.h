#pragma once

#include <iostream>
#include <vector>

namespace nvector {
template <class T>
inline void Write(const std::vector<T>& v) {
  for (const T& t : v) std::cout << t << " ";
  std::cout << std::endl;
}
}  // namespace nvector
