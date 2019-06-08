#pragma once

#include "common/base.h"
#include <vector>

class UnsignedSet {
 protected:
  std::vector<unsigned> vset;
  std::vector<unsigned> vlist;

 public:
  UnsignedSet(unsigned total_size) : vset(total_size, 0) {
    vlist.reserve(total_size);
  };

  bool Empty() const { return vlist.empty(); }
  unsigned Size() const { return unsigned(vlist.size()); }
  unsigned HasKey(unsigned key) const { return vset[key]; }
  unsigned Last() const { return vlist.back(); }

  void Insert(unsigned key) {
    if (!vset[key]) {
      vlist.push_back(key);
      vset[key] = vlist.size();
    }
  }

  void Remove(unsigned key) {
    unsigned p = vset[key];
    if (p) {
      vset[key] = 0;
      if (p != vlist.size()) {
        unsigned q = vlist.back();
        vlist[p - 1] = q;
        vset[q] = p;
      }
      vlist.pop_back();
    }
  }

  void RemoveLast() {
    unsigned u = Last();
    vlist.pop_back();
    vset[u] = 0;
  }

  void Clear() {
    for (unsigned u : vlist) vset[u] = 0;
    vlist.clear();
  }
};
