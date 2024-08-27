#pragma once

#include <algorithm>
#include <vector>

namespace ds {
class DisjointSet {
 protected:
  unsigned n;
  std::vector<unsigned> p;
  std::vector<unsigned> rank;
  std::vector<unsigned> vsize;
  unsigned unions;

 public:
  constexpr explicit DisjointSet(unsigned n = 0) { Init(n); }

  constexpr unsigned Size() const { return n; }

  constexpr unsigned GetUnions() const { return unions; }

  constexpr unsigned GetSetsCount() const { return n - unions; }

  constexpr void Union(unsigned i1, unsigned i2) { UnionI(Find(i1), Find(i2)); }

  constexpr unsigned GetSize(unsigned x) { return vsize[Find(x)]; }

  constexpr void Init(unsigned n_) {
    n = n_;
    p.resize(n);
    for (unsigned i = 0; i < n; ++i) p[i] = i;
    rank.resize(n);
    std::fill(rank.begin(), rank.end(), 0);
    vsize.resize(n);
    std::fill(vsize.begin(), vsize.end(), 1);
    unions = 0;
  }

  constexpr unsigned Find(unsigned x) {
    unsigned px = p[x], ppx = p[px];
    for (; px != ppx;) {
      p[x] = ppx;
      x = ppx;
      px = p[x];
      ppx = p[px];
    }
    return px;
  }

  constexpr std::vector<unsigned> GetRepresentatives() const {
    std::vector<unsigned> output;
    for (unsigned i = 0; i < n; ++i) {
      if (p[i] == i) output.push_back(i);
    }
    return output;
  }

 protected:
  constexpr void UnionI(unsigned i1, unsigned i2) {
    if (i1 == i2) return;
    ++unions;
    if (rank[i1] > rank[i2]) {
      p[i2] = i1;
      vsize[i1] += vsize[i2];
    } else {
      p[i1] = i2;
      if (rank[i1] == rank[i2]) ++rank[i1];
      vsize[i2] += vsize[i1];
    }
  }
};
}  // namespace ds
