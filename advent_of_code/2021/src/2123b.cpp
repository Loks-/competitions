#include "common/geometry/d2/utils/neighbors.h"
#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/hash.h"
#include "common/hash/pair.h"
#include "common/hash/vector.h"
#include "common/heap.h"
#include "common/stl/full.h"
#include "common/stl/pair.h"
#include "common/stl/pair_io.h"
#include "common/string/utils/split.h"
#include "common/vector/read.h"
#include "common/vector/read_all.h"
#include "common/vector/read_from_line.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"
#include "common/vector/write.h"

using Pos = vector<pair<unsigned, unsigned>>;

int main_2123b() {
  vector<unsigned> vcost{1, 10, 100, 1000};
  DHash<Pos> hh;
  unsigned D = 4;
  unsigned N = 11 + 4 * D;
  UndirectedGraph g(N);
  for (unsigned i = 1; i < 11; ++i) g.AddEdge(i - 1, i);
  for (unsigned j = 0; j < 4; ++j) {
    g.AddEdge(D * j + 11, 2 * j + 2);
    for (unsigned d = 1; d < D; ++d) g.AddEdge(D * j + 10 + d, D * j + 11 + d);
  }
  vector<unsigned> poss_stops{0, 1, 3, 5, 7, 9, 10};
  vector<vector<unsigned>> vvd;
  for (unsigned u = 0; u < N; ++u) vvd.push_back(DistanceFromSource(g, u));
  auto vs = nvector::ReadLines();
  Pos vpos(4 * D);
  vector<unsigned> vlf(4, 0);
  for (unsigned l = 0; l < D; ++l) {
    unsigned j = 0;
    for (auto c : vs[2 + l]) {
      if ((c == '#') || (c == ' ')) continue;
      unsigned ll = unsigned(c - 'A');
      vpos[D * ll + vlf[ll]++].second = 11 + l + D * j++;
    }
  }

  auto Sort = [&](Pos& p) {
    for (unsigned i = 0; i < 4; ++i)
      sort(p.begin() + D * i, p.begin() + D * (i + 1));
  };

  auto Target = [&](unsigned i, unsigned d) { return 11 + i * D + d; };

  auto Expected = [&](const Pos& p) {
    unsigned c = 0;
    for (unsigned i = 0; i < 4; ++i) {
      unsigned f = 0;
      for (unsigned d = 0; d < D; ++d) {
        if (p[D * i + d].first == 2) continue;
        c += vcost[i] * vvd[p[D * i + d].second][Target(i, f++)];
      }
    }
    return c;
  };

  auto PossibleToMove = [&](const Pos& p, unsigned from, unsigned to) {
    vector<unsigned> v;
    for (; from != to;) {
      auto d = vvd[from][to];
      for (auto u : g.Edges(from)) {
        if (vvd[u][to] == d - 1) {
          v.push_back(u);
          from = u;
          break;
        }
      }
    }
    sort(v.begin(), v.end());
    for (auto up : p) {
      if (binary_search(v.begin(), v.end(), up.second)) return false;
    }
    return true;
  };

  unordered_map<size_t, unsigned> min_cost;
  unordered_set<size_t> done;
  HeapMinOnTop<pair<unsigned, Pos>> q;

  for (unsigned id = 0; id < vpos.size(); ++id) {
    if (vpos[id].second == Target(id / 4, D - 1)) vpos[id].first = 2;
  }
  for (auto p : vpos) cout << p.first << " " << p.second << endl;
  Sort(vpos);
  auto hpos = hh(vpos);
  min_cost[hpos] = 0;
  q.Add({Expected(vpos), vpos});

  for (; !q.Empty();) {
    vpos = q.Extract().second;
    hpos = hh(vpos);
    auto cost = min_cost[hpos];
    if (done.find(hpos) != done.end()) continue;
    done.insert(hpos);
    if (Expected(vpos) == 0) {
      cout << cost << endl;
      break;
    }

    for (unsigned id = 0; id < vpos.size(); ++id) {
      if (vpos[id].first == 0) {
        for (auto t : poss_stops) {
          if (PossibleToMove(vpos, vpos[id].second, t)) {
            auto new_cost = cost + vvd[vpos[id].second][t] * vcost[id / D];
            auto vpos2 = vpos;
            vpos2[id].first += 1;
            vpos2[id].second = t;
            Sort(vpos2);
            auto h2 = hh(vpos2);
            auto it = min_cost.find(h2);
            if ((it == min_cost.end()) || (it->second > new_cost)) {
              min_cost[h2] = new_cost;
              q.Add({new_cost + Expected(vpos2), vpos2});
            }
          }
        }
      } else if (vpos[id].first == 1) {
        auto t = Target(id / D, D - 1);
        for (unsigned i = id / 4, d = 0; d < D; ++d) {
          if (vpos[D * i + d].first == 2) t -= 1;
        }
        if (PossibleToMove(vpos, vpos[id].second, t)) {
          auto new_cost = cost + vvd[vpos[id].second][t] * vcost[id / D];
          auto vpos2 = vpos;
          vpos2[id].first += 1;
          vpos2[id].second = t;
          Sort(vpos2);
          auto h2 = hh(vpos2);
          auto it = min_cost.find(h2);
          if ((it == min_cost.end()) || (it->second > new_cost)) {
            min_cost[h2] = new_cost;
            q.Add({new_cost + Expected(vpos2), vpos2});
          }
        }
      }
    }
    cout << q.Size() << " " << min_cost.size() << "  " << done.size() << " "
         << cost << endl;
  }
  cout << "Done." << endl;
  return 0;
}
