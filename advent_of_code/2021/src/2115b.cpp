#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/distance_positive_cost.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2115b() {
  auto vs = nvector::ReadLines();
  auto n = vs.size(), m = vs[0].size(), n5 = n * 5, m5 = m * 5;
  DirectedGraphEI<unsigned> g(n5 * m5);
  for (unsigned i = 0; i < n5; ++i) {
    for (unsigned j = 0; j < m5; ++j) {
      unsigned c = (((vs[i % n][j % n] - '1') + (i / n) + (j / m)) % 9) + 1;
      if (i > 0) g.AddEdge((i - 1) * m5 + j, i * m5 + j, c);
      if (i + 1 < n5) g.AddEdge((i + 1) * m5 + j, i * m5 + j, c);
      if (j > 0) g.AddEdge(i * m5 + j - 1, i * m5 + j, c);
      if (j + 1 < m5) g.AddEdge(i * m5 + j + 1, i * m5 + j, c);
    }
  }
  cout << DistanceFromSourcePositiveCost(g, 0, -1u).back() << endl;
  return 0;
}
