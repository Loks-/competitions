#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace ds {
template <class TValue = int64_t>
class BIT_3D {
 protected:
  size_t size1, size2, size3;
  std::vector<TValue> values;

 public:
  BIT_3D(size_t _size1, size_t _size2, size_t _size3)
      : size1(_size1), size2(_size2), size3(_size3) {
    values.resize(size1 * size2 * size3);
  }

  void Reset(size_t new_size1, size_t new_size2, size_t new_size3) {
    size1 = new_size1;
    size2 = new_size2;
    size3 = new_size3;
    values.resize(size1 * size2 * size3);
    std::fill(values.begin(), values.end(), TValue(0));
  }

  void Add(size_t index1, size_t index2, size_t index3,
           TValue value = TValue(1)) {
    for (size_t i1 = index1; i1 < size1; i1 |= (i1 + 1)) {
      for (size_t i2 = index2; i2 < size2; i2 |= (i2 + 1)) {
        for (size_t i3 = index3; i3 < size3; i3 |= (i3 + 1)) {
          values[i3 + size3 * (i2 + size2 * i1)] += value;
        }
      }
    }
  }

  // Get sum of all elements before end.
  TValue Sum(size_t end1, size_t end2, size_t end3) const {
    TValue sum = TValue();
    for (size_t j1, i1 = end1; i1; i1 &= j1) {
      j1 = i1 - 1;
      for (size_t j2, i2 = end2; i2; i2 &= j2) {
        j2 = i2 - 1;
        for (size_t j3, i3 = end3; i3; i3 &= j3) {
          j3 = i3 - 1;
          sum += values[j3 + size3 * (j2 + size2 * j1)];
        }
      }
    }
    return sum;
  }

  TValue Sum(size_t b1, size_t b2, size_t b3, size_t e1, size_t e2,
             size_t e3) const {
    return Sum(e1, e2, e3) - Sum(e1, e2, b3) - Sum(e1, b2, e3) +
           Sum(e1, b2, b3) - Sum(b1, e2, e3) + Sum(b1, e2, b3) +
           Sum(b1, b2, e3) - Sum(b1, b2, b3);
  }

  TValue Get(size_t index1, size_t index2, size_t index3) const {
    return Sum(index1, index2, index3, index1 + 1, index2 + 1, index3 + 1);
  }
};
}  // namespace ds
