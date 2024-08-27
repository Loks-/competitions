#pragma once

#include "common/data_structures/mdvector.h"

namespace ds {
template <class TValue>
constexpr size_t MinIndex(const MDVector<TValue>& v) {
  size_t min_index = 0;
  for (size_t i = 1; i < v.Size(); ++i) {
    if (v[i] < v[min_index]) min_index = i;
  }
  return min_index;
}
}  // namespace ds
