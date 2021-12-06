#pragma once

#include <vector>

namespace nvector {
// First inpt is vector value and second input is index.
template <class TInput, class TFunction>
inline auto Transform(const std::vector<TInput>& v, TFunction f) {
  std::vector<decltype(f(v[0], 0u))> vo;
  for (unsigned i = 0; i < v.size(); ++i) vo.push_back(f(v[i], i));
  return vo;
}
}  // namespace nvector
