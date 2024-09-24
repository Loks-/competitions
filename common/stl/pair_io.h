#pragma once

#include <iostream>
#include <utility>

/**
 * @brief Overload of the extraction operator for std::pair.
 *
 * This function reads a pair of values from an input stream. It extracts the
 * first and second elements of the pair from the stream.
 *
 * @tparam T1 The type of the first element of the pair.
 * @tparam T2 The type of the second element of the pair.
 * @param s The input stream from which to read the pair.
 * @param p The pair to be read from the input stream.
 * @return The input stream after reading the pair.
 */
template <class T1, class T2>
inline std::istream& operator>>(std::istream& s, std::pair<T1, T2>& p) {
  s >> p.first >> p.second;
  return s;
}
