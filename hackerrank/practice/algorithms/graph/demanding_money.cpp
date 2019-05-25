// https://www.hackerrank.com/challenges/borrowing-money

#include "common/graph/graph.h"
#include "common/graph/graph/connected_components_mask.h"
#include "common/stl/base.h"

#include <functional>
#include <unordered_map>

int main_demanding_money() {
  const uint64_t one = 1;
  unsigned N, M;
  cin >> N >> M;
  vector<unsigned> vm(N);
  for (unsigned i = 0; i < N; ++i) {
    cin >> vm[i];
  }
  UndirectedGraph g(N);
  g.ReadEdges(M);

  std::function<pair<unsigned, uint64_t>(uint64_t)> SolveGraph, SolveComponent;
  unordered_map<uint64_t, pair<unsigned, uint64_t>> cache;

  SolveGraph = [&](uint64_t mask) -> pair<unsigned, uint64_t> {
    auto it = cache.find(mask);
    if (it != cache.end()) return it->second;
    auto vc = ConnectedComponentsMask(g, mask);
    unsigned max_value = 0;
    uint64_t ways = 1;
    for (uint64_t c : vc) {
      auto p = SolveComponent(c);
      max_value += p.first;
      ways *= p.second;
    }
    auto output = make_pair(max_value, ways);
    cache[mask] = output;
    return output;
  };

  SolveComponent = [&](uint64_t mask) -> pair<unsigned, uint64_t> {
    if (mask == 0) return {0, 1};
    for (unsigned i = 0; i < N; ++i) {
      uint64_t ibit = (1ull << i);
      if ((1ull << i) & mask) {
        mask &= ~ibit;
        auto p1 = SolveGraph(mask);
        for (unsigned j : g.Edges(i)) mask &= ~(1ull << j);
        auto p2 = SolveGraph(mask);
        p2.first += vm[i];
        if (p1.first < p2.first)
          return p2;
        else if (p2.first < p1.first)
          return p1;
        else
          return {p1.first, p1.second + p2.second};
      }
    }
    return {0, 1};
  };

  auto p = SolveGraph((one << N) - 1);
  cout << p.first << " " << p.second << endl;
  return 0;
}
