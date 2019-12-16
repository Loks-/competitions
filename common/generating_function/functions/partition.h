#pragma once

#include "common/generating_function/generating_function.h"
#include "common/numeric/utils/usqrt.h"

// https://oeis.org/A000041
// https://en.wikipedia.org/wiki/Partition_function_(number_theory)

namespace gf {
namespace functions {
template <class TValue>
class Partition : public GeneratingFunction<TValue> {
 protected:
  std::vector<TValue> va;

 public:
  Partition() : va({1}) {}

  void Adjust(uint64_t n) override {
    for (uint64_t i = va.size(); i <= n; ++i) {
      TValue s = TValue(0);
      uint64_t k1, k2;
      TValue n1 = -TValue(1), m = n1;
      for (uint64_t k = 1;; ++k) {
        k1 = k * (3 * k - 1) / 2;
        k2 = k * (3 * k + 1) / 2;
        m *= n1;
        if (k2 > i) break;
        s += m * (va[i - k2] + va[i - k1]);
      }
      if (k1 <= i) s += m * va[i - k1];
      va.push_back(s);
    }
  }

  TValue Get(uint64_t n) override {
    Adjust(n);
    return va[n];
  }
};

template <class TValue>
static PGeneratingFunction<TValue> CreatePartition() {
  return std::make_shared<Partition<TValue>>();
}
}  // namespace functions
}  // namespace gf
