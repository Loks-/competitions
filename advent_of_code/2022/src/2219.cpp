#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/extract_int.h"
#include "common/vector/make.h"
#include "common/vector/read_lines.h"
#include "common/vector/unique.h"

int main_2219() {
  auto Solve = [](string s, unsigned n) {
    auto vt = nvector::ExtractInt<unsigned>(s, " ");
    auto vcost = nvector::Make<unsigned>(4, 4, 0u);
    // TODO: simplify
    vcost[0][0] = vt[0];
    vcost[1][0] = vt[1];
    vcost[2][0] = vt[2];
    vcost[2][1] = vt[3];
    vcost[3][0] = vt[4];
    vcost[3][2] = vt[5];
    vector<unsigned> vm{max(max(vcost[1][0], vcost[2][0]), vcost[3][0]),
                        vcost[2][1], vcost[3][2], n};
    vector<vector<unsigned>> vv1, vv2;
    vv2.push_back({1, 0, 0, 0, 0, 0, 0, 0});
    for (unsigned t = 0; t < n; ++t) {
      vv1.clear();
      for (const auto& v : vv2) {
        bool ok = true;
        for (const auto& v2 : vv2) {
          if (v == v2) continue;
          bool ok2 = false;
          for (unsigned i = 0; i < 8; ++i) {
            if (v[i] > v2[i]) {
              ok2 = true;
              break;
            }
          }
          if (!ok2) {
            ok = false;
            break;
          }
        }
        if (ok) vv1.push_back(v);
      }
      nvector::UniqueUnsorted(vv1);
      vv2.clear();
      for (const auto& v : vv1) {
        for (unsigned i = 0; i <= 4; ++i) {
          auto vv = v;
          if (i < 4) {
            bool enough = (vv[i] < vm[i]);
            for (unsigned j = 0; j < 4; ++j) {
              if (vv[j + 4] < vcost[i][j]) enough = false;
            }
            if (!enough) continue;
            for (unsigned j = 0; j < 4; ++j) {
              vv[j + 4] -= vcost[i][j];
            }
            vv[i] += 1;
            vv[i + 4] -= 1;
          }
          for (unsigned j = 0; j < 4; ++j) {
            vv[j + 4] += vv[j];
          }
          vv2.push_back(vv);
        }
      }
    }
    unsigned best = 0;
    for (auto& v : vv2) best = max(best, v[7]);
    return best;
  };

  auto vs = nvector::ReadLines();
  unsigned r1 = 0, r2 = 1;
  for (unsigned i = 0; i < vs.size(); ++i) r1 += Solve(vs[i], 24) * (i + 1);
  for (unsigned i = 0; i < 3; ++i) r2 *= Solve(vs[i], 32);
  cout << r1 << endl << r2 << endl;
  return 0;
}
