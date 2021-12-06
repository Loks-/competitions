#pragma once

#include <vector>

namespace nvector {
template <class TInput, class TFunction>
inline auto TransformV(const std::vector<TInput>& v, TFunction f) {
  std::vector<decltype(f(v[0]))> vo;
  for (const auto& vi : v) vo.push_back(f(vi));
  return vo;
}

template <class TInput, class TFunction>
inline auto TransformVI(const std::vector<TInput>& v, TFunction f) {
  std::vector<decltype(f(v[0], 0u))> vo;
  for (unsigned i = 0; i < v.size(); ++i) vo.push_back(f(v[i], i));
  return vo;
}
}  // namespace nvector
