#pragma once

#include <algorithm>
#include <vector>

namespace ds {
namespace fus {
// Memory      -- O(U)
// Init        -- O(U)
// Insert      -- O(U)
// Has         -- O(1)
// Delete      -- O(U)
// Size        -- O(1)
// Successor   -- O(1)
// Predecessor -- O(1)
class VectorPrecomputed {
 protected:
  std::vector<size_t> vnext, vprev;
  size_t size;
  const size_t missed = -size_t(1);

 public:
  void Clear() {
    std::fill(vnext.begin(), vnext.end(), missed);
    std::fill(vprev.begin(), vprev.end(), missed);
    size = 0;
  }

  void Init(size_t u) {
    vnext.clear();
    vnext.resize(u, missed);
    vprev.clear();
    vprev.resize(u, missed);
    size = 0;
  }

  void Insert(size_t x) {
    if (!Has(x)) {
      auto xp = Predecessor(x), xn = Successor(x);
      for (auto y = ((xp == missed) ? 0 : xp); y < x; ++y) vnext[y] = x;
      for (auto y = x + 1, z = ((xn == missed) ? vprev.size() : xn + 1); y < z;
           ++y)
        vprev[y] = x;
      ++size;
    }
  }

  bool Has(size_t x) const {
    return (x ? (vnext[x - 1] == x) : (vprev[1] == 0));
  }

  void Delete(size_t x) {
    if (Has(x)) {
      auto xp = Predecessor(x), xn = Successor(x);
      for (auto y = ((xp == missed) ? 0 : xp); y < x; ++y) vnext[y] = xn;
      for (auto y = x + 1, z = ((xn == missed) ? vprev.size() : xn + 1); y < z;
           ++y)
        vprev[y] = xp;
      --size;
    }
  }

  size_t Size() const { return size; }
  size_t USize() const { return vnext.size(); }

  size_t Successor(size_t x) const { return vnext[x]; }
  size_t Predecessor(size_t x) const { return vprev[x]; }
};
}  // namespace fus
}  // namespace ds
