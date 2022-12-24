#include "common/geometry/d2/utils/neighbors.h"
#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/numeric/utils/lcm.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2224() {
  auto vs = nvector::ReadLines();
  unsigned ny = vs.size() - 2, nx = vs[0].size() - 2, nxy = LCM(nx, ny);
  unsigned ny1 = ny + 2, nx1 = nx + 2;
  vector<unsigned> v(nx1 * ny1 * nxy, 0);
  auto Get = [&](unsigned i, unsigned j, unsigned t) {
    return t + nxy * (j + nx1 * i);
  };
  for (unsigned i = 0; i < vs.size(); ++i) {
    for (unsigned j = 0; j < vs[i].size(); ++j) {
      for (unsigned t = 0; t < nxy; ++t) {
        if (vs[i][j] == '#') {
          v[Get(i, j, t)] += 1;
        } else if (vs[i][j] == '^') {
          v[Get(((i + nxy - t - 1) % ny) + 1, j, t)] += 1;
        } else if (vs[i][j] == 'v') {
          v[Get(((i + t - 1) % ny) + 1, j, t)] += 1;
        } else if (vs[i][j] == '>') {
          v[Get(i, ((j + t - 1) % nx) + 1, t)] += 1;
        } else if (vs[i][j] == '<') {
          v[Get(i, ((j + nxy - t - 1) % nx) + 1, t)] += 1;
        }
      }
    }
  }
  auto vd = I2NeighborsD4();
  vd.push_back({});
  DirectedGraph g(v.size());
  for (unsigned i = 0; i < vs.size(); ++i) {
    for (unsigned j = 0; j < vs[i].size(); ++j) {
      for (unsigned t = 0; t < nxy; ++t) {
        auto k = Get(i, j, t);
        if (v[k]) continue;
        for (auto d : vd) {
          unsigned ii = i + d.dy, jj = j + d.dx;
          if ((ii >= ny1) || (jj >= nx1)) continue;
          auto kk = Get(ii, jj, (t + 1) % nxy);
          if (v[kk] == 0) g.AddEdge(k, kk);
        }
      }
    }
  }
  auto ps = Get(0, 1, 0), pe = Get(ny1 - 1, nx1 - 2, 0), t0 = 0u;
  for (unsigned r = 1; r <= 3; ++r) {
    auto gd = DistanceFromSource(g, ps + (t0 % nxy));
    unsigned best = (1u << 30);
    for (unsigned t = 0; t < nxy; ++t) best = min(best, gd[pe + t]);
    t0 += best;
    swap(pe, ps);
    if (r & 1) cout << t0 << endl;
  }
  return 0;
}
