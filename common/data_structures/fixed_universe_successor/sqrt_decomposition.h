#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/data_structures/fixed_universe_successor/vector_multiset.h"
#include "common/data_structures/fixed_universe_successor/vector_set.h"
#include "common/numeric/utils/usqrt.h"

#include <vector>

namespace ds {
namespace fus {
// Memory      -- O(U)
// Init        -- O(U)
// Insert      -- O(1)
// HasKey      -- O(1)
// Delete      -- O(1)
// Size        -- O(1)
// Successor   -- O(sqrt U)
// Predecessor -- O(sqrt U)
class SqrtDecomposition {
 protected:
  using THigh = VectorMultiset;
  using TLow = VectorSet;

  size_t usize, usize_sqrt;
  THigh vhigh;
  std::vector<TLow> vlow;

 public:
  void Clear() {
    vhigh.Clear();
    for (auto& l : vlow) l.Clear();
  }

  void Init(size_t u) {
    assert(u > 0);
    usize = u;
    usize_sqrt = USqrt(usize);
    size_t ll = (usize - 1) / usize_sqrt + 1;
    vhigh.Init(ll);
    vlow.resize(ll);
    for (auto& l : vlow) l.Init(usize_sqrt);
  }

  void Insert(size_t x) {
    auto xh = x / usize_sqrt, xl = x % usize_sqrt;
    if (!HasKey(xh, xl)) {
      vhigh.Insert(xh);
      vlow[xh].Insert(xl);
    }
  }

  bool HasKey(size_t xh, size_t xl) const { return vlow[xh].HasKey(xl); }

  bool HasKey(size_t x) const { return HasKey(x / usize_sqrt, x % usize_sqrt); }

  void Delete(size_t x) {
    auto xh = x / usize_sqrt, xl = x % usize_sqrt;
    if (HasKey(xh, xl)) {
      vhigh.Delete(xh);
      vlow[xh].Delete(xl);
    }
  }

  size_t Size() const { return vhigh.Size(); }
  size_t USize() const { return usize; }

  size_t Successor(size_t x) const {
    auto xh = x / usize_sqrt, xl = x % usize_sqrt;
    auto rl = vlow[xh].Successor(xl);
    if (rl != Empty) return xh * usize_sqrt + rl;
    xh = vhigh.Successor(xh);
    return (xh == Empty) ? Empty : xh * usize_sqrt + vlow[xh].First();
  }

  size_t Predecessor(size_t x) const {
    auto xh = x / usize_sqrt, xl = x % usize_sqrt;
    auto rl = vlow[xh].Predecessor(xl);
    if (rl != Empty) return xh * usize_sqrt + rl;
    xh = vhigh.Predecessor(xh);
    return (xh == Empty) ? Empty : xh * usize_sqrt + vlow[xh].Last();
  }
};
}  // namespace fus
}  // namespace ds
