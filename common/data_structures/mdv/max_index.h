#pragma once

#include "common/data_structures/mdvector.h"

namespace ds {
template <class TValue>
constexpr size_t MaxIndex(const MDVector<TValue>& v) {
  size_t max_index = 0;
  for (size_t i = 1; i < v.Size(); ++i) {
    if (v[max_index] < v[i]) max_index = i;
  }
  return max_index;
}
}  // namespace ds
