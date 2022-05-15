#include "common/stl/base.h"
#include "common/stl/pair_io.h"
#include "common/vector/make.h"
#include "common/vector/read.h"

#include <unordered_map>

int main_i_o_bot() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int64_t C;
    cin >> N >> C;
    auto vi = nvector::Read<pair<int64_t, int>>(N);
    for (auto& p : vi) p.first *= 2;
    int64_t s = 0;
    sort(vi.begin(), vi.end());
    for (unsigned ip = 0; ip < 2; ++ip) {
      unsigned p0 = 0;
      for (; (p0 < vi.size()) && (vi[p0].first <= 0);) ++p0;
      vector<int64_t> v0, v1, v0s(1, 0), v1s(1, 0);
      for (unsigned p = p0; p < vi.size(); ++p)
        (vi[p].second ? v1 : v0).push_back(vi[p].first);
      for (auto x : v0) v0s.push_back(x + v0s.back());
      for (auto x : v1) v1s.push_back(x + v1s.back());
      int balance = 0;
      unordered_map<int, unsigned> lbm;
      lbm[0] = 0;
      vector<int64_t> vd(vi.size() - p0 + 1, 0);
      for (unsigned i = 0, j = 0, k = 1; k < vd.size(); ++k) {
        auto x = vi[k - 1 + p0].first;
        auto color = vi[k - 1 + p0].second;
        vd[k] = vd[k - 1] + x;
        balance += 2 * color - 1;
        (color ? j : i)++;
        if (k > 1) {
          if (vi[k - 2 + p0].second + color == 1) {
            vd[k] = vd[k - 2] + x;
          } else {
            vd[k] = min(vd[k], vd[k - 2] + x + C);
            if (lbm.find(balance) != lbm.end()) {
              auto k0 = lbm[balance], d = (k - k0) / 2;
              if (color) {
                vd[k] = min(vd[k], vd[k0] + v1s[j] - v1s[j - d]);
              } else {
                vd[k] = min(vd[k], vd[k0] + v0s[i] - v0s[i - d]);
              }
            }
          }
        }
        lbm[balance] = k;
      }
      s += vd.back();
      for (auto& p : vi) p.first = -p.first;
      reverse(vi.begin(), vi.end());
    }
    cout << "Case #" << it << ": " << s << endl;
  }
  return 0;
}
