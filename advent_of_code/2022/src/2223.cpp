#include "common/geometry/d2/stl_hash/point.h"
#include "common/geometry/d2/utils/box.h"
#include "common/geometry/d2/utils/neighbors.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2223() {
  auto vs = nvector::ReadLines();
  auto vd4 = I2NeighborsD4();
  vector<unsigned> vdo{3, 1, 2, 0};
  unordered_map<I2Point, I2Point> elfs;
  unordered_map<I2Point, unsigned> mc;
  for (unsigned i = 0; i < vs.size(); ++i) {
    for (unsigned j = 0; j < vs[i].size(); ++j) {
      if (vs[i][j] == '#') elfs[{j, i}] = {j, i};
    }
  }
  for (unsigned t = 0;; ++t) {
    mc.clear();
    for (auto& it : elfs) {
      auto ep = it.first;
      bool found = false;
      for (auto p : I2NeighborsD8(ep)) {
        if (elfs.find(p) != elfs.end()) {
          found = true;
          break;
        }
      }
      if (found) {
        for (unsigned id = 0; id < 4; ++id) {
          auto d = vdo[(t + id) % 4];
          auto dd = vd4[d];
          bool ok = true;
          if (dd.dx != 0) {
            for (dd.dy = -1; ok && (dd.dy < 2); ++dd.dy) {
              if (elfs.find(ep + dd) != elfs.end()) ok = false;
            }
          } else {
            for (dd.dx = -1; ok && (dd.dx < 2); ++dd.dx) {
              if (elfs.find(ep + dd) != elfs.end()) ok = false;
            }
          }
          if (ok) {
            it.second = ep + vd4[d];
            mc[it.second] += 1;
            break;
          }
        }
      }
    }
    vector<pair<I2Point, I2Point>> moved;
    for (auto& it : elfs) {
      if (it.first == it.second) continue;
      if (mc[it.second] == 1) {
        moved.push_back({it.first, it.second});
      } else {
        it.second = it.first;
      }
    }
    if (moved.empty()) {
      cout << t + 1 << endl;
      break;
    }
    for (auto p : moved) {
      elfs.erase(p.first);
      elfs[p.second] = p.second;
    }
    if (t == 9) {
      vector<I2Point> v;
      for (auto it : elfs) v.push_back(it.first);
      auto b = Box(v);
      auto bd = b.p2 - b.p1;
      cout << (bd.dx + 1) * (bd.dy + 1) - elfs.size() << endl;
    }
  }
  return 0;
}
