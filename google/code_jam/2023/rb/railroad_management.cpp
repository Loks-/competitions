#include "common/stl/base.h"
#include "common/vector/read.h"

#include <queue>

int main_railroad_management() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    vector<unsigned> vd = nvector::Read<unsigned>(N), vi(N, 0), vl;
    vector<int64_t> vc = nvector::Read<int64_t>(N), v0(N, 0);
    int64_t r = 0, z = 0;
    for (auto& d : vd) ++vi[--d];
    queue<unsigned> q;
    for (unsigned i = 0; i < N; ++i) {
      if (vi[i] == 0) q.push(i);
    }
    for (; !q.empty();) {
      auto i = q.front();
      q.pop();
      r += max(vc[i] - v0[i], z);
      auto d = vd[i];
      v0[d] += vc[i];
      if (--vi[d] == 0) q.push(d);
    }
    for (unsigned i = 0; i < N; ++i) {
      if (vi[i]) {
        vl.push_back(i);
        for (auto j = vd[i]; j != i; j = vd[j]) vl.push_back(j);
        int64_t r0 = 1 << 30;
        for (auto j : vl) r0 = min(r0, vc[j] - v0[j]);
        for (auto j : vl) v0[vd[j]] += vc[j];
        for (auto j : vl) r += max(vc[j] - v0[j], z);
        r += max(r0, z);
        for (auto j : vl) vi[j] = 0;
        vl.clear();
      }
    }
    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
