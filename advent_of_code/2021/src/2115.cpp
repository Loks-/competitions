#include "common/geometry/d2/utils/neighbors_graph.h"
#include "common/graph/graph/distance_weighted_vertices_positive_cost.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2115() {
  auto vs = nvector::ReadLines();
  for (unsigned k : {1, 5}) {
    auto n = vs.size(), m = vs[0].size(), nk = n * k, mk = m * k;
    auto g = NeighborsUGraphD4(mk, nk);
    vector<unsigned> vc(nk * mk);
    for (unsigned i = 0, ij = 0; i < nk; ++i) {
      for (unsigned j = 0; j < mk; ++j)
        vc[ij++] = (((vs[i % n][j % m] - '1') + (i / n) + (j / m)) % 9) + 1;
    }
    cout << DistanceFromSourceWeightedVerticesPositiveCost(g, vc, 0, -1u).back()
         << endl;
  }
  return 0;
}
