// https://www.hackerrank.com/challenges/dijkstrashortreach

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/bellman_ford.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/stl/base.h"

int main_dijkstra_shortest_reach_2__bm() {
  unsigned T, max_cost = -1u;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned n, m, k;
    cin >> n >> m;
    UndirectedGraphEI<unsigned> g(n);
    g.ReadEdges(m);
    cin >> k;
    auto vd = graph::distance::BellmanFord(g, graph::EdgeCostProxy<unsigned>(),
                                           k - 1, max_cost);
    for (unsigned i = 0; i < n; ++i) {
      if (i == k - 1) continue;
      cout << ((vd[i] == max_cost) ? -1 : int(vd[i])) << " ";
    }
    cout << endl;
  }
  return 0;
}
