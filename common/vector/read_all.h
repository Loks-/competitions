#pragma once

#include <iostream>
#include <vector>

namespace nvector {

/**
 * @brief Reads all input values of type T from standard input into a vector.
 *
 * This function reads values of type T from standard input (cin) until the end
 * of input and stores them in a vector.
 *
 * @tparam T The type of the elements to read.
 * @return A vector containing all the input values of type T.
 */
template <class T>
inline std::vector<T> ReadAll() {
  T x;
  std::vector<T> v;
  while (std::cin >> x) {
    v.push_back(x);
  }
  return v;
}

}  // namespace nvector
