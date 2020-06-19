#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace ds {
class UnsignedSet {
 protected:
  std::vector<unsigned> vset;
  std::vector<unsigned> vlist;

 public:
  UnsignedSet() {}

  explicit UnsignedSet(unsigned total_size) : vset(total_size, 0) {
    vlist.reserve(total_size);
  };

  bool Empty() const { return vlist.empty(); }
  unsigned Size() const { return unsigned(vlist.size()); }
  unsigned SetSize() const { return unsigned(vset.size()); }
  unsigned HasKey(unsigned key) const { return vset[key]; }
  unsigned Last() const { return vlist.back(); }
  const std::vector<unsigned>& List() const { return vlist; }

  void Insert(unsigned key) {
    if (!vset[key]) {
      vlist.push_back(key);
      vset[key] = vlist.size();
    }
  }

  void InsertAll() {
    unsigned size = SetSize();
    vlist.resize(size);
    for (unsigned i = 0; i < size; ++i) {
      vset[i] = i + 1;
      vlist[i] = i;
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

  void Resize(unsigned new_size) {
    Clear();
    vset.resize(new_size);
    vlist.reserve(new_size);
  }

  void Swap(UnsignedSet& s) {
    vset.swap(s.vset);
    vlist.swap(s.vlist);
  }
};
}  // namespace ds
