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

class Pos {
 public:
  vector<unsigned> vpos;
  vector<unsigned> vm;

  bool operator<(const Pos&) const { return false; }

  bool Done() const {
    for (unsigned i = 0; i < 8; ++i) {
      if (vpos[i] != 11 + i) return false;
    }
    return true;
  }
};

int main_2123a() {
  vector<unsigned> vcost{1, 1, 10, 10, 100, 100, 1000, 1000};
  unsigned N = 19;
  UndirectedGraph g(N);
  for (unsigned i = 1; i < 11; ++i) g.AddEdge(i - 1, i);
  for (unsigned j = 0; j < 4; ++j) {
    g.AddEdge(2 * j + 11, 2 * j + 12);
    g.AddEdge(2 * j + 11, 2 * j + 2);
  }
  vector<unsigned> poss_stops{0, 1, 3, 5, 7, 9, 10};
  vector<vector<unsigned>> vvd;
  for (unsigned u = 0; u < N; ++u) vvd.push_back(DistanceFromSource(g, u));
  auto vs = nvector::ReadLines();
  vector<unsigned> vpos(8);
  vector<unsigned> vlf(4, 0);
  for (unsigned l = 0; l < 2; ++l) {
    unsigned j = 0;
    for (auto c : vs[2 + l]) {
      if ((c == '#') || (c == ' ')) continue;
      unsigned ll = unsigned(c - 'A');
      vpos[2 * ll + vlf[ll]++] = 11 + l + 2 * j++;
    }
  }

  auto Hash = [](const Pos& p) {
    return nhash::HValue<nhash::PolicySafe>(std::make_pair(p.vpos, p.vm));
  };

  auto Expected = [&](const Pos& p) {
    unsigned c = 0;
    for (unsigned i = 0; i < 8; ++i) c += vcost[i] * vvd[p.vpos[i]][11 + i];
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
    for (auto u : p.vpos) {
      if (binary_search(v.begin(), v.end(), u)) return false;
    }
    return true;
  };

  unordered_map<size_t, unsigned> min_cost;
  unordered_set<size_t> done;
  HeapMinOnTop<pair<unsigned, Pos>> q;
  nvector::Write(vpos);

  for (unsigned itry = 0; itry < 16; ++itry) {
    for (unsigned j = 0; j < 4; ++j) {
      if (itry & (1u << j)) swap(vpos[2 * j], vpos[2 * j + 1]);
    }
    Pos p{vpos, vector<unsigned>(8, 0)};
    auto h = Hash(p);
    min_cost[h] = 0;
    q.Add({Expected(p), p});
    for (unsigned j = 0; j < 4; ++j) {
      if (itry & (1u << j)) swap(vpos[2 * j], vpos[2 * j + 1]);
    }
  }

  for (; !q.Empty(); q.Pop()) {
    pair<unsigned, Pos> qt = q.Top();
    auto p = qt.second;
    auto h = Hash(p);
    auto cost = min_cost[h];
    if (done.find(h) != done.end()) continue;
    done.insert(h);
    if (p.Done()) {
      cout << cost << endl;
      break;
    }
    for (unsigned i = 0; i < 8; ++i) {
      if (p.vm[i] == 0) {
        for (auto t : poss_stops) {
          if (PossibleToMove(p, p.vpos[i], t)) {
            auto new_cost = cost + vvd[p.vpos[i]][t] * vcost[i];
            Pos p2 = p;
            p2.vm[i] += 1;
            p2.vpos[i] = t;
            auto h2 = Hash(p2);
            auto it = min_cost.find(h2);
            if ((it == min_cost.end()) || (it->second > new_cost)) {
              min_cost[h2] = new_cost;
              q.Add({new_cost + Expected(p2), p2});
            }
          }
        }
      } else if (p.vm[i] == 1) {
        auto t = i + 11;
        if (PossibleToMove(p, p.vpos[i], t)) {
          auto new_cost = cost + vvd[p.vpos[i]][t] * vcost[i];
          Pos p2 = p;
          p2.vm[i] += 1;
          p2.vpos[i] = t;
          auto h2 = Hash(p2);
          auto it = min_cost.find(h2);
          if ((it == min_cost.end()) || (it->second > new_cost)) {
            min_cost[h2] = new_cost;
            q.Add({new_cost + Expected(p2), p2});
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
