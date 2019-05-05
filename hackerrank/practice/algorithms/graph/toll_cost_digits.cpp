// https://www.hackerrank.com/challenges/toll-cost-digits

#include "common/graph/graph.h"
#include "common/graph/graph/connected_components.h"
#include "common/stl/base.h"
#include "common/vector/unsigned_set.h"

int main_toll_cost_digits() {
  unsigned n, m;
  cin >> n >> m;
  UndirectedGraph g(10 * n);
  for (unsigned i = 0; i < m; ++i) {
    unsigned x, y, p;
    cin >> x >> y >> p;
    for (unsigned d = 0; d < 10; ++d) {
      g.AddEdge(10 * (x - 1) + d, 10 * (y - 1) + ((d + p) % 10));
    }
  }
  UnsignedSet us(n);
  vector<uint64_t> vd(10), vdc(10);
  auto gcc = ConnectedComponents(g);
  for (const auto& gc : gcc) {
    fill(vdc.begin(), vdc.end(), 0);
    for (unsigned u : gc) {
      ++vdc[u % 10];
      if ((u % 10) == 0) us.Insert(u / 10);
    }
    for (unsigned d = 0; d < 10; ++d) vd[d] += vdc[0] * vdc[d];
    for (unsigned u : gc) {
      if (us.HasKey(u / 10)) --vd[u % 10];
    }
    us.Clear();
  }
  for (unsigned i = 0; i < 10; ++i) cout << vd[i] << endl;
  return 0;
}
