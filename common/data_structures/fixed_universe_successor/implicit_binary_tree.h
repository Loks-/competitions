#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <vector>

namespace ds {
namespace fus {
// S < 2^32
// Memory      -- O(U)
// Init        -- O(U)
// Insert      -- O(log U)
// HasKey      -- O(1)
// Delete      -- O(log U)
// Size        -- O(1)
// Min         -- O(log U)
// Max         -- O(log U)
// Successor   -- O(log U)
// Predecessor -- O(log U)
class ImplicitBinaryTree {
 protected:
  std::vector<unsigned> vs;
  size_t usize, usize_adj;

 public:
  void Clear() { std::fill(vs.begin(), vs.end(), 0); }

  void Init(size_t u) {
    usize = u;
    usize_adj = u ? (1ull << (numeric::ULog2(uint64_t(u - 1)) + 1)) : 1ull;
    vs.clear();
    vs.resize(2 * usize_adj, 0);
  }

  void Insert(size_t x) {
    x += usize_adj;
    if (vs[x] == 0) {
      for (; x; x /= 2) ++vs[x];
    }
  }

  bool HasKey(size_t x) const { return vs[x + usize_adj]; }

  void Delete(size_t x) {
    x += usize_adj;
    if (vs[x]) {
      for (; x; x /= 2) --vs[x];
    }
  }

  size_t Size() const { return vs[1]; }
  size_t USize() const { return usize; }

 protected:
  size_t MinI(size_t x) const {
    for (; x < usize_adj;) {
      x *= 2;
      if (!vs[x]) ++x;
    }
    return x - usize_adj;
  }

  size_t MaxI(size_t x) const {
    for (; x < usize_adj;) {
      x = 2 * x + 1;
      if (!vs[x]) --x;
    }
    return x - usize_adj;
  }

 public:
  size_t Min() const { return Size() ? MinI(1) : Empty; }
  size_t Max() const { return Size() ? MaxI(1) : Empty; }

  size_t Successor(size_t x) const {
    x += usize_adj + 1;
    for (; x & (x - 1);) {
      if (vs[x]) return MinI(x);
      x = x / 2 + (x & 1);
    }
    return Empty;
  }

  size_t Predecessor(size_t x) const {
    x += usize_adj - 1;
    for (; x & (x + 1);) {
      if (vs[x]) return MaxI(x);
      x = x / 2 + (x & 1) - 1;
    }
    return Empty;
  }
};
}  // namespace fus
}  // namespace ds
