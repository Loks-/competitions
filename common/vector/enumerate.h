#pragma once

#include <vector>

namespace nvector {
template <class TValue>
inline std::vector<TValue> Enumerate(TValue begin, TValue end,
                                     TValue step = TValue(1)) {
  std::vector<TValue> output;
  for (; begin < end; begin += step) output.push_back(begin);
  return output;
}
}  // namespace nvector
