#include "common/binary_search_tree/treap.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <queue>

using TTree = bst::Treap<true, false, TEmpty>;
using TNode = TTree::TNode;

int main_evolutionary_algorithms__treap() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    auto vs = nvector::Read<int64_t>(N);
    auto vss = vs;
    sort(vss.begin(), vss.end());
    auto vp = nvector::Read<unsigned>(N - 1);
    vector<unsigned> vc(N, 0);
    for (auto p : vp) vc[p - 1] += 1;

    TTree tree(N);
    TNode *p1 = nullptr, *p2 = nullptr;
    uint64_t r = 0;
    vector<TNode *> vn(N);
    queue<unsigned> q;
    for (unsigned i = 0; i < N; ++i) {
      vn[i] = tree.New({}, vs[i] * N + i);
      if (vc[i] == 0) q.push(i);
    }
    for (; !q.empty(); q.pop()) {
      auto i = q.front();
      auto s = vs[i];
      if (s > K) {
        auto s1 = (s - 1) / K;
        uint64_t tt = upper_bound(vss.begin(), vss.end(), s1) - vss.begin();
        if (tt > 1) {
          // Use order instead of split?
          TTree::SplitByKey(vn[i], (s1 + 1) * N, p1, p2);
          uint64_t tc = (p1 ? p1->info.size : size_t(0));
          r += tc * (tt - tc);
          vn[i] = TTree::Join(p1, p2);
        }
      }
      if (i > 0) {
        unsigned j = vp[i - 1] - 1;
        vn[j] = TTree::Union(vn[j], vn[i]);
        vc[j] -= 1;
        if (vc[j] == 0) q.push(j);
      }
    }
    cout << "Case #" << it << ": " << r << endl;
    tree.ReleaseTree(vn[0]);
  }
  return 0;
}
