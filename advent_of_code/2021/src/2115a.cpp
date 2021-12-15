#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance_positive_cost.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2115a() {
  auto vs = nvector::ReadLines();
  auto n = vs.size(), m = vs[0].size();
  DirectedGraphEI<unsigned> g(n * m);
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = 0; j < m; ++j) {
      unsigned c = (vs[i][j] - '0');
      if (i > 0) g.AddEdge((i - 1) * m + j, i * m + j, c);
      if (i + 1 < n) g.AddEdge((i + 1) * m + j, i * m + j, c);
      if (j > 0) g.AddEdge(i * m + j - 1, i * m + j, c);
      if (j + 1 < m) g.AddEdge(i * m + j + 1, i * m + j, c);
    }
  }
  cout << DistanceFromSourcePositiveCost(g, 0, -1u).back() << endl;
  return 0;
}
