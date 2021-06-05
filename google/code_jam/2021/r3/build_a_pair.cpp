#include "common/stl/base.h"
#include "common/numeric/utils/pow.h"

int main_build_a_pair() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    string s;
    cin >> s;
    vector<unsigned> v;
    for (char c : s) v.push_back(c - '0');
    if (v.size() & 1) {
      std::sort(v.begin(), v.end());
      unsigned l = v.size(), l0 = lower_bound(v.begin(), v.end(), 1u) - v.begin();
      uint64_t u0 = 0, u1 = v[l0];
      v.erase(v.begin() + l0);
      for (unsigned i = 0; i < l / 2; ++i) {
        u0 = 10 * u0 + v[l - i - 2];
        u1 = 10 * u1 + v[i];
      }
      cout << "Case #" << it << ": " << u1 - u0 << endl;
    } else {
      uint64_t u0 = 0, u1 = 0, ud = PowU(10ull, v.size() / 2);
      vector<unsigned> vd(11, 0), vc(11, 0), vt;
      vd[10] = 2;
      for (unsigned u : v) vd[u] += 1;
      for (; vc[10] == 0;) {
        bool f1 = false, fm = true;
        for (unsigned d = 1; d < 10; ++d) {
          if (vc[d]) f1 = true;
          if (vc[d] != vd[d]) fm = false;
        }
        if ((vc[0] == 0) || f1) {
          if (fm) {
            ud = 0;
            break;
          }
          for (unsigned d0 = (f1 ? 0 : 1); d0 < 10; ++d0) {
            if (vc[d0] == vd[d0]) continue;
            ++vc[d0];
            for (unsigned d1 = d0 + 1; d1 < 10; ++d1) {
              if (vc[d1] == vd[d1]) continue;
              ++vc[d1];
              u0 = d0;
              u1 = d1;
              vt.clear();
              for (unsigned d = 0; d < 10; ++d) vt.insert(vt.end(), vd[d] - vc[d], d);
              for (unsigned i = 0; i < vt.size() / 2; ++i) {
                u0 = 10 * u0 + vt[vt.size() - i - 1];
                u1 = 10 * u1 + vt[i];
              }
              ud = min(ud, u1 - u0);
              --vc[d1];
              break;
            }
            --vc[d0];
          }
        } 
        for (unsigned i = 0;; ++i) {
          if (vc[i] + 2 <= vd[i]) {
            vc[i] += 2;
            for (unsigned j = 0; j < i; ++j) vc[j] = 0;
            break;
          }
        }
      }
      cout << "Case #" << it << ": " << ud << endl;
    }
  }
  return 0;
}
