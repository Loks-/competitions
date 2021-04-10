#pragma once

#include "common/base.h"
#include "common/numeric/bits/ulog2.h"
#include "common/vector/rmq/position_value.h"

#include <vector>

namespace nvector {
namespace rmq {
// Precompute all intervals whose length is a power of two.
// O(N log N) memory, O(N log N) preprocessing time, O(1) request time
template <class TTValue>
class PPT {
 public:
  using TValue = TTValue;
  using TPositionValue = PositionValue<TValue>;

 protected:
  std::vector<std::vector<TPositionValue>> vpv;

 public:
  PPT() {}
  PPT(const std::vector<TValue>& v) { Build(v); }

  void Build(const std::vector<TValue>& v) {
    size_t n = v.size();
    size_t k = numeric::ULog2(n);
    vpv.resize(k + 1);
    vpv[0].resize(n);
    for (size_t i = 0; i < n; ++i) vpv[0][i] = {i, v[i]};
    for (unsigned k = 1; k < vpv.size(); ++k) {
      size_t p2k1 = (size_t(1) << (k - 1)), p2k = 2 * p2k1;
      vpv[k].resize(n + 1 - p2k);
      for (size_t i = 0; i < n + 1 - p2k; ++i)
        vpv[k][i] = Merge(vpv[k - 1][i], vpv[k - 1][i + p2k1]);
    }
  }

  TPositionValue Minimum(size_t b, size_t e) const {
    assert(b < e);
    unsigned k = numeric::ULog2(e - b);
    return Merge(vpv[k][b], vpv[k][e - (size_t(1) << k)]);
  }
};
}  // namespace rmq
}  // namespace nvector
