// https://www.hackerrank.com/challenges/the-quickest-way-up

#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/stl/base.h"
#include "common/vector/enumerate.h"

int main_snakes_and_ladders_the_quickest_way_up() {
  unsigned T, n = 101, max_cost = 200;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    auto v = Enumerate(0u, n);
    for (unsigned i = 0; i < 2; ++i) {
      unsigned m, x, y;
      cin >> m;
      for (unsigned j = 0; j < m; ++j) {
        cin >> x >> y;
        v[x] = y;
      }
    }
    DirectedGraph g(n);
    for (unsigned i = 1; i < n; ++i) {
      for (unsigned j = 1; j <= 6; ++j) {
        if (i + j >= n) continue;
        g.AddEdge(i, v[i + j]);
      }
    }
    auto vd = DistanceFromSource(g, 1u);
    cout << (vd.back() < max_cost ? int(vd.back()) : -1) << endl;
  }
  return 0;
}
