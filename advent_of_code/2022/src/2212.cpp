#include "common/graph/graph.h"
#include "common/graph/graph/distance_to_sink.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2212() {
  auto vs = nvector::ReadLines();
  auto n = vs.size(), m = vs[0].size(), nm = n * m;
  DirectedGraph g(nm);
  unsigned s = 0, e = 0;
  char c1 = char(1);
  for (unsigned k = 0; k < nm; ++k) {
    auto& c = vs[k / m][k % m];
    if (c == 'S') {
      s = k;
      c = 'a';
    } else if (c == 'E') {
      e = k;
      c = 'z';
    }
  }
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = 0; j < m; ++j) {
      unsigned k = i * m + j;
      if ((i > 0) && (vs[i - 1][j] <= vs[i][j] + c1)) g.AddEdge(k, k - m);
      if ((j > 0) && (vs[i][j - 1] <= vs[i][j] + c1)) g.AddEdge(k, k - 1);
      if ((i + 1 < n) && (vs[i + 1][j] <= vs[i][j] + c1)) g.AddEdge(k, k + m);
      if ((j + 1 < m) && (vs[i][j + 1] <= vs[i][j] + c1)) g.AddEdge(k, k + 1);
    }
  }
  auto vd = DistanceToSink(g, e);
  cout << vd[s] << endl;
  unsigned p2 = vd[s];
  for (unsigned k = 0; k < nm; ++k) {
    if (vs[k / m][k % m] == 'a') p2 = min(p2, vd[k]);
  }
  cout << p2 << endl;
  return 0;
}
