#pragma once

#include <iostream>
#include <vector>

template <class T>
inline void WriteVector(const std::vector<T>& v) {
  for (const T& t : v) std::cout << t << " ";
  std::cout << std::endl;
}
