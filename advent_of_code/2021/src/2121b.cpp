#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2121b() {
  auto vss = nvector::ReadLines();
  unsigned ms = 21;
  vector<uint64_t> vc(1ull << 19, 0);
  auto H = [](unsigned p1, unsigned p2, unsigned m, unsigned s1, unsigned s2) {
    return (p1 << 15) + (p2 << 11) + (m << 10) + (s1 << 5) + s2;
  };
  vc[H(vss[0].back() - '1', vss[1].back() - '1', 0, 0, 0)] = 1;
  for (unsigned s1 = 0; s1 < ms; ++s1) {
    for (unsigned s2 = 0; s2 < ms; ++s2) {
      for (unsigned m = 0; m < 2; ++m) {
        for (unsigned p1 = 0; p1 < 10; ++p1) {
          for (unsigned p2 = 0; p2 < 10; ++p2) {
            uint64_t h = H(p1, p2, m, s1, s2);
            if (vc[h] == 0) continue;
            for (unsigned ic = 0; ic < 27; ++ic) {
              unsigned c = 3 + (ic % 3) + ((ic / 3) % 3) + (ic / 9);
              if (m) {
                auto p2n = (p2 + c) % 10;
                auto s2n = min(s2 + p2n + 1, ms);
                vc[H(p1, p2n, m ^ 1, s1, s2n)] += vc[h];
              } else {
                auto p1n = (p1 + c) % 10;
                auto s1n = min(s1 + p1n + 1, ms);
                vc[H(p1n, p2, m ^ 1, s1n, s2)] += vc[h];
              }
            }
          }
        }
      }
    }
  }
  vector<uint64_t> vw(2, 0);
  for (unsigned p = 0; p < 2; ++p) {
    for (unsigned pp = 0; pp < 10; ++pp) {
      for (unsigned op = 0; op < 10; ++op) {
        for (unsigned os = 0; os < ms; ++os) {
          vw[p] += vc[p ? H(op, pp, p ^ 1, os, ms) : H(pp, op, p ^ 1, ms, os)];
        }
      }
    }
  }
  cout << max(vw[0], vw[1]) << endl;
  return 0;
}
