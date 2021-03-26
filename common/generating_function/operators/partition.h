#pragma once

#include "common/base.h"
#include "common/generating_function/generating_function.h"

#include <vector>

namespace gf {
namespace operators {
template <class TValue>
class Partition : public GeneratingFunction<TValue> {
 protected:
  PGeneratingFunction<TValue> f;
  std::vector<std::vector<TValue>> vcache;

 protected:
  TValue GetI(uint64_t n, uint64_t k) const {
    return vcache[n][std::min(n, k)];
  }

 public:
  explicit Partition(PGeneratingFunction<TValue> _f)
      : f(_f), vcache({{TValue(1)}}) {}

  void Adjust(uint64_t n) override {
    f->Adjust(n);
    while (vcache.size() <= n) {
      uint64_t m = vcache.size();
      std::vector<TValue> vm(m + 1, TValue(0));
      for (uint64_t k = 1; k <= m; ++k) {
        TValue s = vm[k - 1], fk = f->Get(k), b = TValue(1);
        for (uint64_t l = 1; l * k <= m; ++l) {
          b *= (fk + l - 1);
          b /= l;
          s += b * GetI(m - l * k, k - 1);
        }
        vm[k] = s;
      }
      vcache.emplace_back(vm);
    }
  }

  TValue Get(uint64_t n) override {
    Adjust(n);
    return GetI(n, n);
  }
};

template <class TValue>
inline PGeneratingFunction<TValue> MakePartition(
    PGeneratingFunction<TValue> f) {
  return std::make_shared<Partition<TValue>>(f);
}

}  // namespace operators
}  // namespace gf
