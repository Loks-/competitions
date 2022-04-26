#include "common/numeric/bits/bits_count.h"
#include "common/stl/base.h"
#include "common/vector/enumerate.h"
#include "common/vector/make.h"
#include "common/vector/max.h"
#include "common/vector/unique.h"

int main_asedatab() {
  unsigned L = 8, M = 256;
  std::vector<unsigned> vl(M), vu(M), vs(M, M), vm(M);
  auto vt = nvector::Make<unsigned>(M, M, L, 0);
  for (unsigned i = 0; i < M; ++i) {
    vl[i] = numeric::BitsCount(i);
    vu[i] = i;
    auto t = i;
    for (unsigned r = 0; r < L; ++r) {
      vu[i] = min(vu[i], t);
      for (unsigned j = 0; j < M; ++j) vt[j][i][r] = j ^ t;
      t = (t >> 1) + ((t & 1) << 7);
    }
  }
  vs[0] = vm[0] = 0;
  for (unsigned s = 1; s < M; ++s) {
    bool found = false;
    for (unsigned i = 0; i < M; ++i) {
      if ((vu[i] != i) || (vs[i] < s)) continue;
      for (unsigned j = 0; j < M; ++j) {
        bool ok = true;
        for (auto v : vt[i][j]) {
          if (vs[vu[v]] >= s) {
            ok = false;
            break;
          }
        }
        if (ok) {
          found = true;
          vs[i] = s;
          vm[i] = j;
          break;
        }
      }
    }
    if (!found) break;
  }

  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int in = 0;
    unsigned n = 0, l = 0;
    vector<unsigned> vc0, vc1, ve(L + 1);
    for (unsigned u = 0; u < M; ++u) {
      if (vu[u] == u) vc0.push_back(u);
    }
    for (unsigned step = 0;; ++step) {
      unsigned best = M;
      for (unsigned u : vc0) {
        if (vs[u] < best) {
          best = vs[u];
          l = vm[u];
        }
      }
      assert(best < M);
      for (unsigned j = 0; j < L; ++j) cout << ((l >> (7 - j)) & 1u);
      cout << endl;
      cin >> in;
      if (in < 0) return 0;
      n = in;
      if (n == 0) break;
      vc1.clear();
      for (unsigned u : vc0) {
        for (auto v : vt[u][l]) {
          if (vl[v] == n) vc1.push_back(vu[v]);
        }
      }
      nvector::UniqueUnsorted(vc1);
      vc0.swap(vc1);
      assert(!vc0.empty());
    }
  }
  return 0;
}
