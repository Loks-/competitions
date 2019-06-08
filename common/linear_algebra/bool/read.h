#pragma once

#include "common/linear_algebra/bool/matrix.h"
#include "common/linear_algebra/bool/vector.h"
#include "common/modular/static/modular_bool_io.h"
#include <iostream>

inline la::VectorBool ReadVectorBool(unsigned size) {
  la::VectorBool v(size);
  ModularBool b;
  for (unsigned i = 0; i < size; ++i) {
    std::cin >> b;
    v.Set(i, b);
  }
  return v;
}

inline la::MatrixBool ReadMatrixBool(unsigned rows, unsigned columns) {
  la::MatrixBool m(rows, columns);
  ModularBool b;
  for (unsigned i = 0; i < rows; ++i) {
    for (unsigned j = 0; j < columns; ++j) {
      std::cin >> b;
      m.Set(i, j, b);
    }
  }
  return m;
}

inline la::MatrixBool ReadMatrixBool(unsigned size) {
  return ReadMatrixBool(size, size);
}
