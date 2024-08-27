#pragma once

#include "common/base.h"

#include <vector>

namespace ds {
class UnsignedMultiSet {
 protected:
  std::vector<unsigned> vcount;
  std::vector<unsigned> vlist;

 public:
  constexpr explicit UnsignedMultiSet(unsigned total_size)
      : vcount(total_size, 0) {
    vlist.reserve(total_size);
  };

  constexpr bool Empty() const { return vlist.empty(); }

  constexpr unsigned Size() const { return unsigned(vlist.size()); }

  constexpr unsigned Count(unsigned key) const { return vcount[key]; }

  constexpr void Increase(unsigned key, unsigned count = 1) {
    if (!vcount[key]) {
      vlist.push_back(key);
    }
    vcount[key] += count;
  }

  // constexpr std::vector<unsigned>& List() { return vlist; }

  constexpr const std::vector<unsigned>& List() const { return vlist; }

  constexpr void Clear() {
    for (unsigned u : vlist) vcount[u] = 0;
    vlist.clear();
  }
};
}  // namespace ds
