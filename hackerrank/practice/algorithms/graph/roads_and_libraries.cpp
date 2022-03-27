// https://www.hackerrank.com/challenges/torque-and-development

#include "common/graph/graph.h"
#include "common/graph/graph/connected_components_count.h"
#include "common/stl/base.h"

int main_roads_and_libraries() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned n, m;
    uint64_t cl, cr;
    cin >> n >> m >> cl >> cr;
    cr = min(cr, cl);
    UndirectedGraph g(n);
    g.ReadEdges(m);
    unsigned gcc = ConnectedComponentsCount(g);
    cout << gcc * cl + (n - gcc) * cr << endl;
  }
  return 0;
}
