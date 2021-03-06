// https://www.hackerrank.com/challenges/subset-component

#include "common/data_structures/disjoint_set.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_subset_component() {
  unsigned n, s = 0;
  cin >> n;
  auto v = nvector::Read<uint64_t>(n);
  vector<vector<unsigned>> vv(n);
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned bit = 0; bit < 64; ++bit) {
      if ((1ull << bit) & v[i]) {
        vv[i].push_back(bit);
      }
    }
  }
  for (unsigned ss = 0; ss < (1u << n); ++ss) {
    ds::DisjointSet djs(64);
    for (unsigned i = 0; i < n; ++i) {
      if ((1u << i) & ss) {
        for (unsigned j = 1; j < vv[i].size(); ++j) {
          djs.Union(vv[i][0], vv[i][j]);
        }
      }
    }
    s += djs.GetSetsCount();
  }
  cout << s << endl;
  return 0;
}
