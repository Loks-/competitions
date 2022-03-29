#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/data_structures/fixed_universe_successor/u64_set.h"
#include "common/data_structures/fixed_universe_successor/vector_multiset.h"
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
// Min         -- O(U)
// Max         -- O(U)
// Successor   -- O(U)
// Predecessor -- O(U)
class TwoLayersU64 {
 protected:
  using THigh = VectorMultiset;
  using TLow = U64Set;

  size_t usize;
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
    size_t ll = ((usize - 1) >> 6) + 1;
    vhigh.Init(ll);
    vlow.resize(ll);
    for (auto& l : vlow) l.Init(64);
  }

  void Insert(size_t x) {
    auto xh = x >> 6, xl = x & 63;
    if (!HasKey(xh, xl)) {
      vhigh.Insert(xh);
      vlow[xh].Insert(xl);
    }
  }

  bool HasKey(size_t xh, size_t xl) const { return vlow[xh].HasKey(xl); }

  bool HasKey(size_t x) const { return HasKey(x >> 6, x & 63); }

  void Delete(size_t x) {
    auto xh = x >> 6, xl = x & 63;
    if (HasKey(xh, xl)) {
      vhigh.Delete(xh);
      vlow[xh].Delete(xl);
    }
  }

  size_t Size() const { return vhigh.Size(); }
  size_t USize() const { return usize; }

  size_t Min() const {
    auto xh = vhigh.Min();
    return (xh == Empty) ? Empty : (xh << 6) + vlow[xh].Min();
  }

  size_t Max() const {
    auto xh = vhigh.Max();
    return (xh == Empty) ? Empty : (xh << 6) + vlow[xh].Max();
  }

  size_t Successor(size_t x) const {
    auto xh = x >> 6, xl = x & 63;
    auto rl = vlow[xh].Successor(xl);
    if (rl != Empty) return (xh << 6) + rl;
    xh = vhigh.Successor(xh);
    return (xh == Empty) ? Empty : (xh << 6) + vlow[xh].Min();
  }

  size_t Predecessor(size_t x) const {
    auto xh = x >> 6, xl = x & 63;
    auto rl = vlow[xh].Predecessor(xl);
    if (rl != Empty) return (xh << 6) + rl;
    xh = vhigh.Predecessor(xh);
    return (xh == Empty) ? Empty : (xh << 6) + vlow[xh].Max();
  }
};
}  // namespace fus
}  // namespace ds
