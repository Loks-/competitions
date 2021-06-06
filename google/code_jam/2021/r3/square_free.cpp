#include "common/graph/flow/edge.h"
#include "common/graph/flow/graph.h"
#include "common/graph/flow/min_cost_flow.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <numeric>

int main_square_free() {
  unsigned T, R, C;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> R >> C;
    auto vr = nvector::Read<int>(R), vc = nvector::Read<int>(C);
    unsigned source = R + C, sink = source + 1;
    FlowGraph<graph::flow::Edge<int, int>> g(sink + 1, source, sink);
    for (unsigned i = 0; i < R; ++i) {
      for (unsigned j = 0; j < C; ++j)
        g.AddDataEdge((i + j) * (i + j), i, R + j, 1);
      g.AddDataEdge(0, source, i, vr[i]);
    }
    for (unsigned j = 0; j < C; ++j) g.AddDataEdge(0, R + j, sink, vc[j]);
    MinCostFlow(g, R * C);
    auto f = g.Flow();
    bool ok = (f == accumulate(vr.begin(), vr.end(), 0)) &&
              (f == accumulate(vc.begin(), vc.end(), 0));
    cout << "Case #" << it << ": " << (ok ? "POSSIBLE" : "IMPOSSIBLE") << endl;
    if (ok) {
      for (unsigned i = 0; i < R; ++i) {
        for (unsigned j = 0; j < C; ++j)
          cout << ((g.Edges(i)[j].flow > 0) ? "/" : "\\");
        cout << endl;
      }
    }
  }
  return 0;
}
