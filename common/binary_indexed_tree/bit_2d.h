#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

template <class TValue = int64_t>
class BIT_2D {
 protected:
  size_t size1, size2;
  std::vector<TValue> values;

 public:
  BIT_2D(size_t _size1, size_t _size2)
      : size1(_size1), size2(_size2) {
    values.resize(size1 * size2);
  }

  void Reset(size_t new_size1, size_t new_size2) {
    size1 = new_size1;
    size2 = new_size2;
    values.resize(size1 * size2);
    std::fill(values.begin(), values.end(), TValue(0));
  }

  void Add(size_t index1, size_t index2,
           TValue value = TValue(1)) {
    for (size_t i1 = index1; i1 < size1; i1 |= (i1 + 1)) {
      for (size_t i2 = index2; i2 < size2; i2 |= (i2 + 1)) {
        values[i2 + size2 * i1] += value;
      }
    }
  }

  // Get sum of all elements before end.
  TValue Sum(size_t end1, size_t end2) const {
    TValue sum = TValue();
    for (size_t j1, i1 = end1; i1; i1 &= j1) {
      j1 = i1 - 1;
      for (size_t j2, i2 = end2; i2; i2 &= j2) {
        j2 = i2 - 1;
        sum += values[j2 + size2 * j1];
      }
    }
    return sum;
  }

  TValue Sum(size_t b1, size_t b2, size_t e1, size_t e2) const {
    return Sum(e1, e2) - Sum(e1, b2) - Sum(b1, e2) + Sum(b1, b2);
  }

  TValue Get(size_t index1, size_t index2) const {
    return Sum(index1, index2, index1 + 1, index2 + 1);
  }
};
