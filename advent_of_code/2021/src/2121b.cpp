#include "common/data_structures/mdvector.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2121b() {
  auto vss = nvector::ReadLines();
  unsigned ms = 21;
  ds::MDVector<uint64_t> vc(0, {10, 10, 2, ms + 1, ms + 1});
  vc(vss[0].back() - '1', vss[1].back() - '1', 0, 0, 0) = 1;
  for (unsigned s1 = 0; s1 < ms; ++s1) {
    for (unsigned s2 = 0; s2 < ms; ++s2) {
      for (unsigned m = 0; m < 2; ++m) {
        for (unsigned p1 = 0; p1 < 10; ++p1) {
          for (unsigned p2 = 0; p2 < 10; ++p2) {
            auto v = vc(p1, p2, m, s1, s2);
            if (v == 0) continue;
            for (unsigned ic = 0; ic < 27; ++ic) {
              unsigned c = 3 + (ic % 3) + ((ic / 3) % 3) + (ic / 9);
              if (m) {
                auto p2n = (p2 + c) % 10;
                auto s2n = min(s2 + p2n + 1, ms);
                vc(p1, p2n, m ^ 1, s1, s2n) += v;
              } else {
                auto p1n = (p1 + c) % 10;
                auto s1n = min(s1 + p1n + 1, ms);
                vc(p1n, p2, m ^ 1, s1n, s2) += v;
              }
            }
          }
        }
      }
    }
  }
  cout << max(vc.DProject(3, ms).Sum(), vc.DProject(4, ms).Sum()) << endl;
  return 0;
}
