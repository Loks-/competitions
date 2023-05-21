#include "common/graph/graph.h"
#include "common/graph/graph/articulation_points.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_railroad_maintenance() {
  unsigned T, N, L, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> L;
    UndirectedGraph g(N + L);
    for (unsigned i = 0; i < L; ++i) {
      cin >> K;
      auto v = nvector::Read<unsigned>(K);
      for (auto u : v) g.AddEdge(i + N, u - 1);
    }
    unsigned r = 0;
    for (auto u : ArticulationPoints(g)) {
      if (u >= N) r += 1;
    }
    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
