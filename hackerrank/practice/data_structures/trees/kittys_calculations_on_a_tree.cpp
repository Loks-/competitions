// https://www.hackerrank.com/challenges/kittys-calculations-on-a-tree

#include "common/graph/tree/centroid_decomposition.h"
#include "common/graph/tree/lca.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

#include <unordered_map>

using graph::CentroidDecomposition;

namespace {
class SolutionKittysCalculationOnATree
    : public graph::ICentroidDecompositionCallBack {
 protected:
  TreeGraph tree;
  graph::LCA lca;
  vector<TModularD> results;
  vector<unordered_map<unsigned, vector<unsigned>>> sets;

 public:
  void Solve() {
    unsigned n, q, k, v;
    cin >> n >> q;
    tree.Resize(n);
    tree.ReadEdges();
    lca.Build(tree);
    sets.resize(n);
    for (unsigned iq = 0; iq < q; ++iq) {
      cin >> k;
      for (unsigned ik = 0; ik < k; ++ik) {
        cin >> v;
        sets[0][iq].push_back(v - 1);
      }
    }
    results.clear();
    results.resize(q);
    CentroidDecomposition::Run(tree, *this);
    for (TModularD r : results) cout << r << endl;
  }

  void PostDecompose(unsigned vertex, const CentroidDecomposition& cd) {
    const vector<unsigned>& group_map = cd.GetGroupMap();
    unsigned current_group = group_map[vertex];
    for (const auto& it : sets[current_group]) {
      const auto& v = it.second;
      if (v.size() <= 1) continue;
      unsigned q = it.first;
      TModularD s, sd;
      unordered_map<unsigned, pair<TModularD, TModularD>> m;
      for (unsigned u : v) {
        TModularD d(lca.GetDistance(u, vertex));
        TModularD w(u + 1), wd = w * d;
        s += w;
        sd += wd;
        unsigned g = group_map[u];
        m[g].first += w;
        m[g].second += wd;
        if (u != vertex) sets[g][q].push_back(u);
      }
      TModularD t = s * sd;
      for (auto& p : m) t -= p.second.first * p.second.second;
      results[q] += t;
    }
  }
};
}  // namespace

int main_kittys_calculations_on_a_tree() {
  SolutionKittysCalculationOnATree s;
  s.Solve();
  return 0;
}
