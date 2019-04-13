#include "common/graph/base_graph.h"
#include "common/hash.h"
#include "common/stl/base.h"

#include <unordered_set>

// bool FindHamiltonPathI(const BaseUndirectedGraph& g,
//                        vector<unsigned>& path_output, vector<bool>& vused) {
//   assert(path_output.size() > 0);
//   unsigned N = g.Size();
//   if (path_output.size() == N) return true;
//   unsigned current = path_output.back();
//   for (unsigned u : g.Edges(current)) {
//     if (!vused[u]) {
//       vused[u] = true;
//       path_output.push_back(u);
//       if (FindHamiltonPathI(g, path_output, vused)) return true;
//       path_output.pop_back();
//       vused[u] = false;
//     }
//   }
//   return false;
// }

unordered_set<size_t> hash_set;

bool FindHamiltonPathI(const BaseUndirectedGraph& g,
                       vector<unsigned>& path_output, vector<bool>& vused) {
  assert(path_output.size() > 0);
  unsigned N = g.Size();
  if (path_output.size() == N) return true;
  unsigned current = path_output.back();
  hash<vector<bool>> hvb;
  size_t h = hash_combine(hvb(vused), current);
  if (hash_set.find(h) != hash_set.end()) return false;
  vector<pair<unsigned, unsigned>> vp;
  for (unsigned u : g.Edges(current)) {
    if (vused[u]) continue;
    unsigned k = 0;
    for (unsigned v : g.Edges(u)) {
      if (!vused[v]) ++k;
    }
    vp.push_back({k, u});
  }
  sort(vp.begin(), vp.end());
  if (vp.size() == 0) return false;
  if ((vp[0].first == 0) && (path_output.size() + 1 < N)) return false;
  for (auto p : vp) {
    unsigned u = p.second;
    vused[u] = true;
    path_output.push_back(u);
    if (FindHamiltonPathI(g, path_output, vused)) return true;
    path_output.pop_back();
    vused[u] = false;
  }
  hash_set.insert(h);
  return false;
}

bool FindHamiltonPath(const BaseUndirectedGraph& g,
                      vector<unsigned>& path_output) {
  hash_set.clear();
  unsigned N = g.Size();
  path_output.clear();
  path_output.reserve(N);
  vector<bool> vused(N, false);
  for (unsigned i = 0; i < N; ++i) {
    path_output.push_back(i);
    vused[i] = true;
    if (FindHamiltonPathI(g, path_output, vused)) return true;
    vused[i] = false;
    path_output.pop_back();
  }
  return false;
}

int main_pylons() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned R, C;
    cin >> R >> C;
    unsigned N = R * C;
    BaseUndirectedGraph g(N);
    for (unsigned i = 0; i < N; ++i) {
      unsigned ir = i / C, ic = i % C;
      for (unsigned j = 0; j < N; ++j) {
        unsigned jr = j / C, jc = j % C;
        if ((ir == jr) || (ic == jc) || (ir + ic == jr + jc) ||
            (ir + jc == ic + jr))
          continue;
        g.AddEdge(i, j);
      }
    }
    vector<unsigned> hpath;
    bool r = FindHamiltonPath(g, hpath);
    cout << "Case #" << it << ": " << (r ? "POSSIBLE" : "IMPOSSIBLE") << endl;
    if (r) {
      for (unsigned u : hpath) {
        cout << u / C + 1 << " " << u % C + 1 << endl;
      }
    }
  }
  return 0;
}
