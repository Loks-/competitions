#pragma once

#include "common/base.h"

#include <vector>

namespace ds {
class UnsignedMultiSet {
 protected:
  std::vector<unsigned> vcount;
  std::vector<unsigned> vlist;

 public:
  explicit UnsignedMultiSet(unsigned total_size) : vcount(total_size, 0) {
    vlist.reserve(total_size);
  };

  bool Empty() const { return vlist.empty(); }
  unsigned Size() const { return unsigned(vlist.size()); }
  unsigned Count(unsigned key) const { return vcount[key]; }

  void Increase(unsigned key, unsigned count = 1) {
    if (!vcount[key]) {
      vlist.push_back(key);
    }
    vcount[key] += count;
  }

  std::vector<unsigned>& List() { return vlist; }
  const std::vector<unsigned>& List() const { return vlist; }

  void Clear() {
    for (unsigned u : vlist) vcount[u] = 0;
    vlist.clear();
  }
};
}  // namespace ds
