#include "common/data_structures/binary_indexed_tree/bit.h"
#include "common/graph/tree.h"
#include "common/graph/tree/nodes_info.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_evolutionary_algorithms() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    auto vs = nvector::Read<int64_t>(N);
    TreeGraph tree(N);
    tree.ReadTreeEdges();
    graph::TreeNodesInfo tni(tree);
    ds::BIT<int64_t> bit(N);
    vector<pair<int64_t, unsigned>> vp;
    for (unsigned i = 0; i < N; ++i) vp.push_back({vs[i], i});
    sort(vp.begin(), vp.end());
    int64_t r = 0;
    for (unsigned i = 0, j = 0; j < vp.size(); ++j) {
      for (; vp[i].first * K < vp[j].first; ++i)
        bit.Add(tni.preorder[vp[i].second]);
      auto u = vp[j].second;
      auto k = bit.Sum(tni.preorder[u], tni.preorder[u] + tni.subtree_size[u]);
      r += k * (int64_t(i) - k);
    }
    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
