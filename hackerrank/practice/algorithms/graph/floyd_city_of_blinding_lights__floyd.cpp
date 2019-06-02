// https://www.hackerrank.com/challenges/floyd-city-of-blinding-lights

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance/floyd_warshall.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/stl/base.h"

int main_floyd_city_of_blinding_lights__floyd() {
  unsigned N, M, Q, j, k, r;
  cin >> N >> M;
  DirectedGraphEI<unsigned> g(N);
  // This problem contains edge duplicates
  // g.ReadEdges(M);
  vector<vector<unsigned>> vt(N, vector<unsigned>(N, 0));
  for (unsigned i = 0; i < M; ++i) {
    cin >> j >> k >> r;
    vt[j - 1][k - 1] = r;
  }
  for (unsigned i = 0; i < N; ++i) {
    for (unsigned j = 0; j < N; ++j) {
      if (vt[i][j]) g.AddEdge(i, j, vt[i][j]);
    }
  }
  vector<vector<unsigned>> vd =
      graph::distance::FloydWarshall(g, graph::EdgeCostProxy<unsigned>(), -1u);
  cin >> Q;
  for (unsigned i = 0; i < Q; ++i) {
    cin >> j >> k;
    cout << int(vd[j - 1][k - 1]) << endl;
  }
  return 0;
}
