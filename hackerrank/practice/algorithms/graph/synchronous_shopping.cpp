// https://www.hackerrank.com/challenges/synchronous-shopping

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance_positive_cost.h"
#include "common/stl/base.h"

int main_synchronous_shopping() {
  unsigned n, m, k;
  cin >> n >> m >> k;
  n += 1;
  unsigned p2k = (1u << k);
  vector<unsigned> vn(n, 0);
  for (unsigned i = 1; i < n; ++i) {
    unsigned t1, t2, u = 0;
    cin >> t1;
    for (unsigned i1 = 0; i1 < t1; ++i1) {
      cin >> t2;
      u |= (1u << (t2 - 1));
    }
    vn[i] = u;
  }
  DirectedGraphEI<unsigned> g(n * p2k);
  for (unsigned i = 0; i < m; ++i) {
    unsigned t1, t2, c;
    cin >> t1 >> t2 >> c;
    for (unsigned j = 0; j < p2k; ++j) {
      g.AddEdge(t1 * p2k + j, t2 * p2k + (j | vn[t2]), c);
      g.AddEdge(t2 * p2k + j, t1 * p2k + (j | vn[t1]), c);
    }
  }
  unsigned max_cost = unsigned(-1), shift = (n - 1) * p2k;
  auto vd = DistanceFromSourcePositiveCost(g, p2k + vn[1], max_cost);
  unsigned best_cost = max_cost;
  for (unsigned j1 = 0; j1 < p2k; ++j1) {
    if (vd[shift + j1] >= best_cost) continue;
    for (unsigned j2 = j1; j2 < p2k; ++j2) {
      if (vd[shift + j2] >= best_cost) continue;
      if ((j1 | j2) == (p2k - 1)) {
        best_cost = min(best_cost, max(vd[shift + j1], vd[shift + j2]));
      }
    }
  }
  cout << best_cost << endl;
  return 0;
}
