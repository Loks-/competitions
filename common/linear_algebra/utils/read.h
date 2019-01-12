#pragma once

#include "../matrix.h"
#include <iostream>

template <class TValue>
inline Matrix<TValue> ReadMatrix(unsigned rows, unsigned columns) {
  Matrix<TValue> m(rows, columns);
  for (typename Matrix<TValue>::iterator it = m.begin(), it_end = m.end();
       it < it_end;)
    std::cin >> *it++;
  return m;
}

template <class TValue>
inline Matrix<TValue> ReadMatrix(unsigned size) {
  return ReadMatrix<TValue>(size, size);
}
