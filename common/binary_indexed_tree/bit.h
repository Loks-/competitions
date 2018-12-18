#pragma once

#include "../base.h"
#include <algorithm>
#include <vector>

// Binary Indexed Tree (same as Fenwick tree)
template <class TValue = int64_t>
class BIT {
 protected:
  unsigned size;
  std::vector<TValue> values;

 public:
  BIT(unsigned _size) : size(_size) { values.resize(size, TValue()); }

  void Reset(unsigned new_size) {
    size = new_size;
    values.resize(size);
    std::fill(values.begin(), values.end(), TValue());
  }

  void Add(unsigned index, TValue value = TValue(1)) {
    for (; index < size; index |= (index + 1)) values[index] += value;
  }

  // Get sum of all elements before end.
  TValue Sum(unsigned end) const {
    TValue sum = TValue();
    for (unsigned index, index1 = end; index1; index1 &= index) {
      index = index1 - 1;
      sum += values[index];
    }
    return sum;
  }

  TValue Sum(unsigned begin, unsigned end) const {
    return Sum(end) - Sum(begin);
  }

  TValue Get(unsigned index) const { return Sum(index, index + 1); }
};
