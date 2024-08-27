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
  constexpr UnsignedSet() {}

  constexpr explicit UnsignedSet(unsigned total_size) : vset(total_size, 0) {
    vlist.reserve(total_size);
  };

  constexpr bool Empty() const { return vlist.empty(); }

  constexpr unsigned Size() const { return unsigned(vlist.size()); }

  constexpr unsigned SetSize() const { return unsigned(vset.size()); }

  constexpr unsigned HasKey(unsigned key) const { return vset[key]; }

  constexpr unsigned Last() const { return vlist.back(); }

  constexpr const std::vector<unsigned>& List() const { return vlist; }

  constexpr void Insert(unsigned key) {
    if (!vset[key]) {
      vlist.push_back(key);
      vset[key] = vlist.size();
    }
  }

  constexpr void InsertAll() {
    const unsigned size = SetSize();
    vlist.resize(size);
    for (unsigned i = 0; i < size; ++i) {
      vset[i] = i + 1;
      vlist[i] = i;
    }
  }

  constexpr void Remove(unsigned key) {
    const unsigned p = vset[key];
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

  constexpr void RemoveLast() {
    const unsigned u = Last();
    vlist.pop_back();
    vset[u] = 0;
  }

  constexpr void Clear() {
    for (unsigned u : vlist) vset[u] = 0;
    vlist.clear();
  }

  constexpr void Resize(unsigned new_size) {
    Clear();
    vset.resize(new_size);
    vlist.reserve(new_size);
  }

  constexpr void Swap(UnsignedSet& s) {
    vset.swap(s.vset);
    vlist.swap(s.vlist);
  }
};
}  // namespace ds
