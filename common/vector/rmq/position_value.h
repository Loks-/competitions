#pragma once

#include "common/base.h"

namespace nvector {
namespace rmq {
template <class TTValue>
class PositionValue {
 public:
  using TValue = TTValue;

  size_t pos;
  TValue value;

  bool operator<(const PositionValue& r) const { return value < r.value; }
};

template <class TValue>
inline const PositionValue<TValue>& Merge(const PositionValue<TValue>& l,
                                          const PositionValue<TValue>& r) {
  return (r.value < l.value) ? r : l;
}
}  // namespace rmq
}  // namespace nvector
