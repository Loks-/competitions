#include "common/data_structures/disjoint_set.h"
#include "common/stl/base.h"
#include "common/vector/unique.h"

#include <unordered_map>

int main_the_decades_of_coding_competitions() {
  class Road {
   public:
    unsigned u, v, k;
  };

  unsigned T, N, M, Q;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> M >> Q;
    ++N;
    vector<Road> vr(M);
    unordered_map<unsigned, unsigned> cm;
    vector<unsigned> uc;
    for (auto& r : vr) {
      cin >> r.u >> r.v >> r.k;
      if (cm.find(r.k) == cm.end()) {
        cm[r.k] = uc.size();
        uc.push_back(r.k);
      }
    }
    auto C = uc.size() + 1;
    vector<ds::DisjointSet> vds(C, ds::DisjointSet(N));
    for (auto& r : vr) {
      auto c = cm[r.k];
      for (unsigned i = 0; i < C; ++i) {
        if (i != c) vds[i].Union(r.u, r.v);
      }
    }
    vector<vector<unsigned>> vvr(C * N);
    for (auto& r : vr) {
      auto c = cm[r.k];
      for (unsigned i = 0; i < C; ++i) {
        if (i != c) vvr[i * N + vds[i].Find(r.u)].push_back(r.k);
      }
    }
    for (auto& v : vvr) nvector::UniqueUnsorted(v);
    unsigned r = 0, u, v;
    for (unsigned q = 0; q < Q; ++q) {
      cin >> u >> v;
      for (unsigned i = 0; i < C; ++i) {
        auto fu = vds[i].Find(u), fv = vds[i].Find(v);
        if (fu != fv) continue;
        if (vvr[i * N + fu].size() & 1) {
          ++r;
          break;
        }
      }
    }
    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
