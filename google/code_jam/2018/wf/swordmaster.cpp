#include "common/data_structures/unsigned_set.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_swordmaster() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n, p;
    cin >> n >> p;
    vector<vector<unsigned>> va(n), vd(n);
    for (unsigned i = 0; i < n; ++i) {
      unsigned a, d;
      cin >> a >> d;
      va[i] = nvector::Read<unsigned>(a);
      vd[i] = nvector::Read<unsigned>(d);
    }
    bool possible = true;
    // Check A
    auto va2 = va;
    vector<unsigned> vd0(p + 1);
    for (auto d : vd[0]) vd0[d] = 1;
    ds::UnsignedSet usd(n);
    usd.InsertAll();
    usd.Remove(0);
    vector<unsigned> vat;
    for (bool bnext = true; bnext && !usd.Empty();) {
      bnext = false;
      auto vl = usd.List();
      for (auto u : vl) {
        for (auto a : va2[u]) {
          if (!vd0[a]) vat.push_back(a);
        }
        if (vat.empty()) {
          bnext = true;
          usd.Remove(u);
          for (auto d : vd[u]) vd0[d] = 1;
        } else {
          va2[u].swap(vat);
          vat.clear();
        }
      }
    }
    if (!usd.Empty()) possible = false;
    // Check D (small test only)
    vector<unsigned> va0(p + 1);
    for (auto a : va[0]) va0[a] = 1;
    usd.InsertAll();
    usd.Remove(0);
    for (unsigned i = 0; i < va[0].size(); ++i) {
      auto a = va[0][i];
      auto vl = usd.List();
      for (auto u : vl) {
        if (!binary_search(vd[u].begin(), vd[u].end(), a)) {
          usd.Remove(u);
          for (auto anew : va[u]) {
            if (va0[anew] == 0) {
              va0[anew] = 1;
              va[0].push_back(anew);
            }
          }
        }
      }
    }
    if (!usd.Empty()) possible = false;
    cout << "Case #" << it << ": " << (possible ? "YES" : "NO") << endl;
  }
  return 0;
}
