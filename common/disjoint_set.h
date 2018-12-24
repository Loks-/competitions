#pragma once

#include <algorithm>
#include <vector>

class DisjointSet {
 protected:
  unsigned n;
  std::vector<unsigned> p;
  std::vector<unsigned> rank;
  std::vector<unsigned> vsize;
  unsigned unions;

 public:
  DisjointSet(unsigned n = 0) { Init(n); }
  unsigned Size() const { return n; }
  unsigned GetUnions() const { return unions; }
  unsigned GetSetsCount() const { return n - unions; }
  void Union(unsigned i1, unsigned i2) { UnionI(Find(i1), Find(i2)); }
  unsigned GetSize(unsigned x) { return vsize[Find(x)]; }

  void Init(unsigned n_) {
    n = n_;
    p.resize(n);
    for (unsigned i = 0; i < n; ++i) p[i] = i;
    rank.resize(n);
    std::fill(rank.begin(), rank.end(), 0);
    vsize.resize(n);
    std::fill(vsize.begin(), vsize.end(), 1);
    unions = 0;
  }

  unsigned Find(unsigned x) {
    unsigned px = p[x], ppx = p[px];
    for (; px != ppx;) {
      p[x] = ppx;
      x = ppx;
      px = p[x];
      ppx = p[px];
    }
    return px;
  }

  std::vector<unsigned> GetRepresentatives() const {
    std::vector<unsigned> output;
    for (unsigned i = 0; i < n; ++i) {
      if (p[i] == i) output.push_back(i);
    }
    return output;
  }

 protected:
  void UnionI(unsigned i1, unsigned i2) {
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
