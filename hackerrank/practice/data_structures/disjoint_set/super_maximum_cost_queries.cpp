// https://www.hackerrank.com/challenges/maximum-cost-queries

#include "common/disjoint_set.h"
#include "common/stl/base.h"

#include <map>

int main_super_maximum_cost_queries() {
  map<unsigned, uint64_t> mpaths;
  mpaths[0] = 0;
  unsigned n, q;
  cin >> n >> q;
  DisjointSet ds(n + 1);
  vector<tuple<unsigned, unsigned, unsigned>> vt;
  for (unsigned i = 1; i < n; ++i) {
    unsigned u, v, w;
    cin >> u >> v >> w;
    vt.push_back(make_tuple(w, u, v));
  }
  sort(vt.begin(), vt.end());
  for (auto t : vt) {
    unsigned r1 = ds.Find(get<1>(t));
    unsigned r2 = ds.Find(get<2>(t));
    if (r1 != r2) {
      mpaths[get<0>(t)] += ds.GetSize(r1) * ds.GetSize(r2);
      ds.Union(r1, r2);
    }
  }
  uint64_t s = 0;
  for (auto it1 = mpaths.begin(); it1 != mpaths.end(); ++it1) {
    s += it1->second;
    it1->second = s;
  }
  for (unsigned i = 0; i < q; ++i) {
    unsigned u, v;
    cin >> u >> v;
    auto it1 = mpaths.lower_bound(u);
    auto it2 = mpaths.lower_bound(v + 1);
    cout << (--it2)->second - (--it1)->second << endl;
  }
  return 0;
}
