#include "common/geometry/d2/utils/neighbors_graph.h"
#include "common/graph/graph/distance_weighted_vertices_positive_cost.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2115a() {
  auto vs = nvector::ReadLines();
  auto n = vs.size(), m = vs[0].size();
  auto g = NeighborsUGraphD4(m, n);
  vector<unsigned> vc(n * m);
  for (unsigned i = 0, ij = 0; i < n; ++i) {
    for (unsigned j = 0; j < m; ++j) vc[ij++] = (vs[i][j] - '0');
  }
  cout << DistanceFromSourceWeightedVerticesPositiveCost(g, vc, 0, -1u).back()
       << endl;
  return 0;
}
