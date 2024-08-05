#pragma once

#include <algorithm>
#include <vector>

namespace nvector {
template <class TValue>
constexpr unsigned LongestIncresingSubsequence(const std::vector<TValue>& v) {
  std::vector<TValue> d;
  for (unsigned i = 0; i < v.size(); ++i) {
    unsigned j = std::lower_bound(d.begin(), d.end(), v[i]) - d.begin();
    if (j == d.size())
      d.push_back(v[i]);
    else
      d[j] = v[i];
  }
  return d.size();
}
}  // namespace nvector
