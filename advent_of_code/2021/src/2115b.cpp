#include "common/geometry/d2/utils/neighbors_graph.h"
#include "common/graph/graph/distance_weighted_vertices_positive_cost.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2115b() {
  auto vs = nvector::ReadLines();
  auto n = vs.size(), m = vs[0].size(), n5 = n * 5, m5 = m * 5;
  auto g = NeighborsUGraphD4(m5, n5);
  vector<unsigned> vc(n5 * m5);
  for (unsigned i = 0, ij = 0; i < n5; ++i) {
    for (unsigned j = 0; j < m5; ++j)
      vc[ij++] = (((vs[i % n][j % m] - '1') + (i / n) + (j / m)) % 9) + 1;
  }
  cout << DistanceFromSourceWeightedVerticesPositiveCost(g, vc, 0, -1u).back()
       << endl;
  return 0;
}
