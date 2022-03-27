#include "common/geometry/d2/point.h"
#include "common/numeric/bits/first_bit.h"
#include "common/stl/base.h"
#include "common/vector/unique.h"

#include <functional>
#include <string>

int main_two_tiling() {
  uint64_t m1 = 255, m2 = (-1ull) / m1, mh = (1ull << 63);
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    string s;
    vector<string> vs;
    for (; vs.size() < 3;) {
      std::getline(std::cin, s);
      if (!s.empty()) vs.push_back(s);
    }
    vector<vector<I2Point>> vp2(2);
    for (unsigned k = 0; k < 2; ++k) {
      for (unsigned i = 0; i < 3; ++i) {
        for (unsigned j = 0; j < 3; ++j) {
          if (vs[i][j + 4 * k] == '@') vp2[k].push_back({i, j});
        }
      }
    }
    vector<vector<uint64_t>> vm2(2);
    for (unsigned k = 0; k < 2; ++k) {
      for (unsigned r = 0; r < 8; ++r) {
        uint64_t m = 0;
        for (auto& p : vp2[k]) m += (1ull << (p.x + 8 * p.y));
        for (; (m & m1) == 0;) m >>= 8;
        for (; (m & m2) == 0;) m >>= 1;
        vm2[k].push_back(m);
        for (auto& p : vp2[k]) {
          auto t = p.x;
          p.x = p.y;
          p.y = 2 - t;
        }
        if (r == 4) {
          for (auto& p : vp2[k]) p.x = 2 - p.x;
        }
      }
      nvector::UniqueUnsorted(vm2[k]);
      unsigned n = vm2[k].size();
      for (unsigned i = 0; i < n; ++i) {
        auto m = vm2[k][i];
        for (unsigned j = 1; (m & mh) == 0; ++j) {
          m <<= 1;
          if (((m & m2) == 0) || ((j % 8) == 0)) vm2[k].push_back(m);
        }
      }
    }

    vector<vector<uint64_t>> vsolution(2);
    vector<uint64_t> cm(2, 0);
    std::function<bool()> Solve = [&]() -> bool {
      if (cm[0] == cm[1]) return true;
      auto l = numeric::Lowest0Bits(cm[0] ^ cm[1]);
      unsigned k = (cm[0] >> l) & 1u;
      auto b = (1ull << l);
      for (auto m : vm2[k]) {
        if ((m & b) && ((cm[k] & m) == 0)) {
          vsolution[k].push_back(m);
          cm[k] += m;
          if (Solve()) return true;
          cm[k] -= m;
          vsolution[k].pop_back();
        }
      }
      return false;
    };

    for (auto m : vm2[0]) {
      auto l = numeric::Lowest0Bits(m);
      if (l >= 8) continue;
      vsolution[0].push_back(m);
      cm[0] = m + (1ull << l) - 1;
      cm[1] = (1ull << l) - 1;
      if (Solve()) break;
      vsolution[0].pop_back();
    }

    bool b = (vsolution[1].size() > 0);
    cout << "Case #" << it << ": " << (b ? "POSSIBLE" : "IMPOSSIBLE") << endl;
    if (b) {
      for (unsigned i = 0; i < 8; ++i) {
        for (unsigned k = 0; k < 2; ++k) {
          for (unsigned j = 0; j < 8; ++j) {
            char c = '.';
            auto bm = (1ull << (8 * i + j));
            for (unsigned l = 0; l < vsolution[k].size(); ++l) {
              if (vsolution[k][l] & bm) c = 'A' + char(l);
            }
            cout << c;
          }
          cout << (k ? "\n" : " ");
        }
      }
    }
  }
  return 0;
}
