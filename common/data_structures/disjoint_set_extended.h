#pragma once

#include <unordered_map>
#include <vector>

namespace ds {
template <class TTData>
class DisjointSetExtended {
 public:
  using TData = TTData;

 protected:
  std::unordered_map<TData, unsigned> m;
  std::vector<unsigned> p;
  std::vector<unsigned> rank;
  std::vector<unsigned> vsize;
  unsigned unions = 0;

 public:
  void Reserve(unsigned size) {
    m.reserve(size);
    p.reserve(size);
    rank.reserve(size);
    vsize.reserve(size);
  }

  void Reset() {
    m.clear();
    p.clear();
    rank.clear();
    vsize.clear();
    unions = 0;
  }

  constexpr unsigned Size() const { return unsigned(p.size()); }

  constexpr unsigned GetUnions() const { return unions; }

  unsigned GetSize(const TData& v) { return vsize[Find(v)]; }

  void Union(const TData& v1, const TData& v2) { UnionI(Find(v1), Find(v2)); }

  unsigned GetIndex(const TData& v) {
    const auto it = m.find(v);
    if (it != m.end()) return it->second;
    const unsigned index = unsigned(Size());
    m[v] = index;
    p.push_back(index);
    rank.push_back(0);
    vsize.push_back(1);
    return index;
  }

  unsigned Find(const TData& v) {
    unsigned x = GetIndex(v);
    unsigned px = p[x], ppx = p[px];
    for (; px != ppx;) {
      p[x] = ppx;
      x = ppx;
      px = p[x];
      ppx = p[px];
    }
    return px;
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
}  // namespace ds
