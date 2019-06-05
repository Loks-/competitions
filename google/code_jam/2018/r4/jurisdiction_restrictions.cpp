#include "common/binary_search.h"
#include "common/coordinate_compression.h"
#include "common/graph/flow/edge.h"
#include "common/graph/flow/graph.h"
#include "common/graph/flow/max_flow.h"
#include "common/stl/base.h"

int main_jurisdiction_restrictions() {
  struct PS {
    int64_t r, rb, re, c, cb, ce;
  };

  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned R, C, S;
    cin >> R >> C >> S;
    vector<PS> stations;
    vector<int64_t> vr, vc;
    for (unsigned i = 0; i < S; ++i) {
      int64_t r, c, d;
      cin >> r >> c >> d;
      PS ps{r, (d < r) ? r - d - 1 : 0, (r + d > R) ? R : r + d,
            c, (d < c) ? c - d - 1 : 0, (c + d > C) ? C : c + d};
      vr.push_back(ps.rb);
      vr.push_back(ps.re);
      vc.push_back(ps.cb);
      vc.push_back(ps.ce);
      stations.push_back(ps);
    }
    CoordinateCompression<int64_t> ccr(vr), ccc(vc);
    unsigned sr = ccr.Size() - 1, sc = ccc.Size() - 1,
             sg = 2 + stations.size() + sr * sc;
    FlowGraph<FlowGraphEdge> g(sg, 0, sg - 1);
    int64_t max_flow =
        (ccr.GetOld(sr) - ccr.GetOld(0)) * (ccc.GetOld(sc) - ccc.GetOld(0));
    for (unsigned i = 0; i < stations.size(); ++i)
      g.AddEdge(0, i + 1, max_flow);
    for (unsigned r = 0; r < sr; ++r) {
      for (unsigned c = 0; c < sc; ++c) {
        unsigned index = 1 + stations.size() + r * sc + c;
        int64_t rb = ccr.GetOld(r), re = ccr.GetOld(r + 1), cb = ccc.GetOld(c),
                ce = ccc.GetOld(c + 1);
        int64_t s = (re - rb) * (ce - cb);
        for (unsigned i = 0; i < stations.size(); ++i) {
          auto ps = stations[i];
          if ((ps.rb <= rb) && (ps.re >= re) && (ps.cb <= cb) && (ps.ce >= ce))
            g.AddEdge(i + 1, index, max_flow);
          if ((ps.r > rb) && (ps.r <= re) && (ps.c > cb) && (ps.c <= ce)) --s;
        }
        g.AddEdge(index, sg - 1, s);
      }
    }
    int64_t g_max_flow = MaxFlow(g);

    auto set_source_flow = [&](unsigned f) {
      g.ResetFlow();
      for (auto& e : g.Edges(0)) e.max_flow = f;
    };
    auto f1 = [&](unsigned f) {
      set_source_flow(f);
      auto gf = MaxFlow(g);
      return gf < f * stations.size();
    };
    auto f2 = [&](unsigned f) {
      set_source_flow(f);
      auto gf = MaxFlow(g);
      return gf == g_max_flow;
    };
    auto t1 = LowerBoundB(int64_t(0), g_max_flow, f1) - 1;
    auto t2 = LowerBoundB(int64_t(0), g_max_flow, f2);
    cout << "Case #" << it << ": " << t2 - t1 << endl;
  }
  return 0;
}
