#pragma once

#include "common/base.h"
#include <vector>

class UnsignedSet {
 protected:
  std::vector<uint8_t> vset;
  std::vector<unsigned> vlist;

 public:
  UnsignedSet(unsigned total_size) : vset(total_size, 0) {
    vlist.reserve(total_size);
  };

  uint8_t HasKey(unsigned key) const { return vset[key]; }

  void Insert(unsigned key) {
    if (!vset[key]) {
      vset[key] = 1;
      vlist.push_back(key);
    }
  }

  unsigned Size() const { return unsigned(vlist.size()); }

  void Clear() {
    for (unsigned u : vlist) vset[u] = 0;
    vlist.clear();
  }
};
