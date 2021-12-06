#pragma once

#include <iostream>
#include <vector>

namespace nvector {
template <class T>
inline std::vector<T> ReadAll() {
  T x;
  std::vector<T> v;
  for (; cin >> x;) v.push_back(x);
  return v;
}
}  // namespace nvector
