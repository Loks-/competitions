#pragma once

#include "common/linear_algebra/matrix.h"
#include <iostream>

namespace la {
template <class TValue>
inline la::Matrix<TValue> ReadMatrix(unsigned rows, unsigned columns) {
  la::Matrix<TValue> m(rows, columns);
  for (typename la::Matrix<TValue>::iterator it = m.begin(), it_end = m.end();
       it < it_end;)
    std::cin >> *it++;
  return m;
}

template <class TValue>
inline la::Matrix<TValue> ReadMatrix(unsigned size) {
  return ReadMatrix<TValue>(size, size);
}
}  // namespace la
