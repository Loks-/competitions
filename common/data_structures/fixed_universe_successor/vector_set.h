#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"

#include <algorithm>
#include <vector>

namespace ds {
namespace fus {
// Memory      -- O(U)
// Init        -- O(U)
// Insert      -- O(1)
// HasKey      -- O(1)
// Delete      -- O(1)
// Size        -- O(1)
// Successor   -- O(U)
// Predecessor -- O(U)
class VectorSet {
 protected:
  std::vector<unsigned> vs;
  size_t size;

 public:
  void Clear() {
    std::fill(vs.begin(), vs.end(), 0);
    size = 0;
  }

  void Init(size_t u) {
    vs.clear();
    vs.resize(u);
    size = 0;
  }

  void Insert(size_t x) {
    if (vs[x] == 0) {
      vs[x] = 1;
      ++size;
    }
  }

  bool HasKey(size_t x) const { return vs[x]; }

  void Delete(size_t x) {
    if (vs[x] == 1) {
      vs[x] = 0;
      --size;
    }
  }

  size_t Size() const { return size; }
  size_t USize() const { return vs.size(); }

  size_t Successor(size_t x) const {
    for (++x; x < vs.size(); ++x) {
      if (vs[x]) return x;
    }
    return Empty;
  }

  size_t Predecessor(size_t x) const {
    for (--x; x < vs.size(); --x) {
      if (vs[x]) return x;
    }
    return Empty;
  }

  size_t First() const {
    for (size_t x = 0; x < vs.size(); ++x) {
      if (vs[x]) return x;
    }
    return Empty;
  }

  size_t Last() const { return Predecessor(vs.size()); }
};
}  // namespace fus
}  // namespace ds
