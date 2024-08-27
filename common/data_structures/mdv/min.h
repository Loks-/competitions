#pragma once

#include "common/base.h"
#include "common/data_structures/mdvector.h"

namespace ds {
template <class TValue>
constexpr MDVector<TValue> Min(const MDVector<TValue>& v1,
                               const MDVector<TValue>& v2) {
  assert(v1.GetShape() == v2.GetShape());
  auto r = v1;
  for (size_t i = 0; i < r.Size(); ++i) {
    if (v2[i] < r[i]) r[i] = v2[i];
  }
  return r;
}
}  // namespace ds
