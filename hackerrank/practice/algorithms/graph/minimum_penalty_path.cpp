// https://www.hackerrank.com/challenges/beautiful-path

#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/stl/base.h"

int main_minimum_penalty_path() {
  unsigned n, m, k, l, max_cost = 1024;
  cin >> n >> m;
  DirectedGraph g((n + 1) * max_cost);
  for (unsigned i = 0; i < m; ++i) {
    unsigned u, v, c;
    cin >> u >> v >> c;
    for (unsigned cc = 0; cc < max_cost; ++cc) {
      g.AddEdge(u * max_cost + cc, v * max_cost + (c | cc));
      g.AddEdge(v * max_cost + cc, u * max_cost + (c | cc));
    }
  }
  cin >> k >> l;
  auto vd = DistanceFromSource(g, k * max_cost);
  unsigned cost = max_cost;
  for (unsigned cc = 0; cc < max_cost; ++cc) {
    if (vd[l * max_cost + cc] != -1u) {
      cost = cc;
      break;
    }
  }
  cout << ((cost == max_cost) ? -1 : int(cost)) << endl;
  return 0;
}
