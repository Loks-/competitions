#include "common/data_structures/unsigned_set.h"
#include "common/geometry/d2/distance.h"
#include "common/geometry/d2/point_io.h"
#include "common/graph/bipartite.h"
#include "common/graph/bipartite/matching.h"
#include "common/stl/base.h"
#include "common/vector/make.h"
#include "common/vector/read.h"

int main_saving_the_jelly() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cout << "Case #" << it << ": ";
    cin >> N;
    auto vp1 = nvector::Read<I2Point>(N);
    auto vp2 = nvector::Read<I2Point>(N + 1);
    auto vvd = nvector::Make<int64_t>(N, N);
    auto vdj = vector<int64_t>(N);
    BipartiteGraph g(N, N);
    for (unsigned i = 0; i < N; ++i) {
      vdj[i] = SquaredDistanceL2(vp1[i], vp2[0]);
      for (unsigned j = 0; j < N; ++j) {
        vvd[i][j] = SquaredDistanceL2(vp1[i], vp2[j + 1]);
        if (vvd[i][j] <= vdj[i]) g.AddEdge(i, j + N);
      }
    }
    graph::BipartiteMatching bm;
    auto m = bm.Matching(g);
    if (m == N) {
      cout << "POSSIBLE" << endl;
      auto vm = bm.Get();
      ds::UnsignedSet s1(N), s2(N), s1t(N);

      auto Add = [&](unsigned i) {
        auto j = vm[i] - N;
        cout << i + 1 << " " << j + 2 << endl;
        s1.Remove(i);
        s2.Remove(j);
      };

      for (s1.InsertAll(), s2.InsertAll(); !s1.Empty();) {
        auto l1 = s1.List();
        for (auto i : l1) {
          bool ok = true;
          for (auto j : s2.List()) {
            if (vvd[i][j] < vvd[i][vm[i] - N]) {
              ok = false;
              break;
            }
          }
          if (ok) Add(i);
        }
        if (l1.size() == s1.Size()) {
          // Loop search required
          s1t.Clear();
          vector<pair<unsigned, unsigned>> vp;
          unsigned i = s1.Last();
          for (; !s1t.HasKey(i);) {
            unsigned js = bm[i] - N;
            for (auto j : s2.List()) {
              if (vvd[i][j] < vvd[i][js]) js = j;
            }
            vp.push_back({i, js});
            s1t.Insert(i);
            i = bm[js + N];
          }
          reverse(vp.begin(), vp.end());
          for (auto& p : vp) {
            vm[p.first] = p.second + N;
            vm[p.second + N] = p.first;
            if (p.first == i) break;
          }
          Add(i);
        }
      }
    } else {
      cout << "IMPOSSIBLE" << endl;
    }
  }
  return 0;
}
