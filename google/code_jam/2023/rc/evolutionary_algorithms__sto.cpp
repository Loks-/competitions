#include "common/data_structures/segment_tree/info/sum.h"
#include "common/graph/tree.h"
#include "common/graph/tree/sto.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TSTO = graph::STO<uint64_t, ds::st::info::Sum<uint64_t>>;

int main_evolutionary_algorithms__sto() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    auto vs = nvector::Read<int64_t>(N);
    TreeGraph tree(N);
    tree.ReadTreeEdges();
    TSTO sto(tree);
    vector<pair<int64_t, unsigned>> vp;
    for (unsigned i = 0; i < N; ++i) vp.push_back({vs[i], i});
    sort(vp.begin(), vp.end());
    uint64_t r = 0;
    for (unsigned i = 0, j = 0; j < vp.size(); ++j) {
      for (; vp[i].first * K < vp[j].first; ++i) sto.SetData(vp[i].second, 1);
      auto k = sto.Subtree(vp[j].second).GetInfo().sum;
      r += k * (i - k);
    }
    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
