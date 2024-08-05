#pragma once

#include <vector>

namespace nvector {
template <class T>
constexpr bool IsStrongSorted(const std::vector<T>& v) {
  for (size_t i = 1; i < v.size(); ++i) {
    if (!(v[i - 1] < v[i])) return false;
  }
  return true;
}

template <class T, class TCompare>
constexpr bool IsStrongSorted(const std::vector<T>& v,
                              const TCompare& cmp_less) {
  for (size_t i = 1; i < v.size(); ++i) {
    if (!cmp_less(v[i - 1], v[i])) return false;
  }
  return true;
}

template <class T>
constexpr bool IsWeakSorted(const std::vector<T>& v) {
  for (size_t i = 1; i < v.size(); ++i) {
    if (v[i] < v[i - 1]) return false;
  }
  return true;
}

template <class T, class TCompare>
constexpr bool IsWeakSorted(const std::vector<T>& v, const TCompare& cmp_less) {
  for (size_t i = 1; i < v.size(); ++i) {
    if (cmp_less(v[i], v[i - 1])) return false;
  }
  return true;
}
}  // namespace nvector
