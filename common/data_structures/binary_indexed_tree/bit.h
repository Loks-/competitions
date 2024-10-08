#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace ds {
// Binary Indexed Tree (same as Fenwick tree)
template <class TValue = int64_t>
class BIT {
 protected:
  size_t size;
  std::vector<TValue> values;

 public:
  constexpr explicit BIT(size_t _size) : size(_size) {
    values.resize(size, TValue());
  }

  constexpr void Reset(size_t new_size) {
    size = new_size;
    values.resize(size);
    std::fill(values.begin(), values.end(), TValue());
  }

  constexpr void Add(size_t index, TValue value = TValue(1)) {
    for (; index < size; index |= (index + 1)) values[index] += value;
  }

  // Get sum of all elements before end.
  constexpr TValue Sum(size_t end) const {
    TValue sum = TValue();
    for (size_t index, index1 = end; index1; index1 &= index) {
      index = index1 - 1;
      sum += values[index];
    }
    return sum;
  }

  constexpr TValue Sum(size_t begin, size_t end) const {
    return Sum(end) - Sum(begin);
  }

  constexpr TValue Get(size_t index) const { return Sum(index, index + 1); }
};
}  // namespace ds
