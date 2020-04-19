#pragma once

#include "common/base.h"

#include <unordered_map>

template <class TValue = int64_t>
class BITSparse {
 protected:
  uint64_t size;
  std::unordered_map<uint64_t, TValue> values;

 public:
  explicit BITSparse(uint64_t _size) : size(_size) {}

  void Reset(uint64_t new_size) {
    size = new_size;
    values.clear();
  }

  void Add(uint64_t index, TValue value = TValue(1)) {
    for (; index < size; index |= (index + 1)) values[index] += value;
  }

  // Get sum of all elements before end.
  TValue Sum(uint64_t end) const {
    TValue sum = TValue();
    for (uint64_t index, index1 = end; index1; index1 &= index) {
      index = index1 - 1;
      auto it = values.find(index);
      if (it != values.end()) sum += it->second;
    }
    return sum;
  }

  TValue Sum(uint64_t begin, uint64_t end) const {
    return Sum(end) - Sum(begin);
  }

  TValue Get(uint64_t index) const { return Sum(index, index + 1); }
};
