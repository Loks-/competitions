#include "common/geometry/d2/utils/neighbors.h"
#include "common/stl/base.h"
#include "common/vector/make.h"
#include "common/vector/read_lines.h"

int main_2223() {
  auto vd4 = I2NeighborsD4();
  vector<unsigned> vdo{3, 1, 2, 0};
  auto vs = nvector::ReadLines();
  unsigned N = 3 * max(vs.size(), vs[0].size()), t = 0;
  auto vm = nvector::Make<unsigned>(N, N);
  vector<I2Point> aelfs, aelfs3;
  vector<pair<I2Point, I2Point>> aelfs4;
  for (unsigned i = 0; i < vs.size(); ++i) {
    for (unsigned j = 0; j < vs[i].size(); ++j) {
      if (vs[i][j] == '#') aelfs.push_back({j + N / 3, i + N / 3});
    }
  }
  unsigned total = aelfs.size();
  for (auto elf : aelfs) vm[elf.x][elf.y] = 10;
  for (; !aelfs.empty(); ++t) {
    aelfs3.clear();
    aelfs4.clear();
    // unsigned es = aelfs.size();
    for (unsigned l = 0; l < aelfs.size(); ++l) {
      auto ep = aelfs[l];
      assert(vm[ep.x][ep.y] == 10);
      bool found = false;
      for (auto p : I2NeighborsD8(ep)) {
        if (vm[p.x][p.y] >= 10) {
          found = true;
          if (vm[p.x][p.y] == 11) {
            vm[p.x][p.y] = 10;
            aelfs.push_back(p);
          }
        }
      }
      if (found) {
        auto ed = ep;
        for (unsigned id = 0; id < 4; ++id) {
          auto d = vdo[(t + id) % 4];
          auto dd = vd4[d];
          bool ok = true;
          if (dd.dx != 0) {
            for (dd.dy = -1; ok && (dd.dy < 2); ++dd.dy) {
              if (vm[ep.x + dd.dx][ep.y + dd.dy] == 10) ok = false;
            }
          } else {
            for (dd.dx = -1; ok && (dd.dx < 2); ++dd.dx) {
              if (vm[ep.x + dd.dx][ep.y + dd.dy] == 10) ok = false;
            }
          }
          if (ok) {
            ed = ep + vd4[d];
            vm[ed.x][ed.y] += 1;
            break;
          }
        }
        if (ed == ep) {
          aelfs3.push_back(ep);
        } else {
          aelfs4.push_back({ep, ed});
        }
      } else {
        vm[ep.x][ep.y] = 11;
      }
    }
    for (auto& p : aelfs4) {
      auto pf = p.first, ps = p.second;
      if (vm[ps.x][ps.y] == 1) {
        vm[pf.x][pf.y] = 0;
        vm[ps.x][ps.y] = 10;
        aelfs3.push_back(ps);
      } else {
        vm[ps.x][ps.y] = 0;
        aelfs3.push_back(pf);
      }
    }
    aelfs.swap(aelfs3);
    if (t == 9) {
      unsigned x1 = N, x2 = 0, y1 = N, y2 = 0;
      for (unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < N; ++j) {
          if (vm[i][j] >= 10) {
            x1 = min(x1, i);
            x2 = max(x2, i);
            y1 = min(y1, j);
            y2 = max(y2, j);
          }
        }
      }
      cout << (x2 - x1 + 1) * (y2 - y1 + 1) - total << endl;
    }
  }
  cout << t << endl;
  return 0;
}
