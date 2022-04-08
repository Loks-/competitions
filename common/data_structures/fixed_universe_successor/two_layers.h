#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/data_structures/fixed_universe_successor/vector_multiset.h"

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
template <class TFLS>
class TwoLayers {
 protected:
  static const unsigned low_level_bits = TFLS::nbits;
  static const size_t mask = (1ull << low_level_bits) - 1;

  size_t usize;
  VectorMultiset vhigh;
  std::vector<TFLS> vlow;

 public:
  void Clear() {
    vhigh.Clear();
    for (auto& l : vlow) l.Clear();
  }

  void Init(size_t u) {
    assert(u > 0);
    usize = u;
    size_t ll = ((usize - 1) >> low_level_bits) + 1;
    vhigh.Init(ll);
    vlow.resize(ll);
    for (auto& l : vlow) l.Init((1ull << low_level_bits));
  }

  void Insert(size_t x) {
    auto xh = x >> low_level_bits, xl = x & mask;
    if (!HasKey(xh, xl)) {
      vhigh.Insert(xh);
      vlow[xh].Insert(xl);
    }
  }

  bool HasKey(size_t xh, size_t xl) const { return vlow[xh].HasKey(xl); }

  bool HasKey(size_t x) const { return HasKey(x >> low_level_bits, x & mask); }

  void Delete(size_t x) {
    auto xh = x >> low_level_bits, xl = x & mask;
    if (HasKey(xh, xl)) {
      vhigh.Delete(xh);
      vlow[xh].Delete(xl);
    }
  }

  size_t Size() const { return vhigh.Size(); }
  size_t USize() const { return usize; }

  size_t Min() const {
    auto xh = vhigh.Min();
    return (xh == Empty) ? Empty : (xh << low_level_bits) + vlow[xh].MinI();
  }

  size_t Max() const {
    auto xh = vhigh.Max();
    return (xh == Empty) ? Empty : (xh << low_level_bits) + vlow[xh].MaxI();
  }

  size_t Successor(size_t x) const {
    auto xh = x >> low_level_bits, xl = x & mask;
    auto rl = vlow[xh].Successor(xl);
    if (rl != Empty) return (xh << low_level_bits) + rl;
    xh = vhigh.Successor(xh);
    return (xh == Empty) ? Empty : (xh << low_level_bits) + vlow[xh].MinI();
  }

  size_t Predecessor(size_t x) const {
    auto xh = x >> low_level_bits, xl = x & mask;
    auto rl = vlow[xh].Predecessor(xl);
    if (rl != Empty) return (xh << low_level_bits) + rl;
    xh = vhigh.Predecessor(xh);
    return (xh == Empty) ? Empty : (xh << low_level_bits) + vlow[xh].MaxI();
  }
};
}  // namespace fus
}  // namespace ds
