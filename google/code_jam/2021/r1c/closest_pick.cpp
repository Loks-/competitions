#include "common/stl/base.h"
#include "common/vector/read.h"

int main_closest_pick() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    auto v = nvector::Read<unsigned>(N);
    sort(v.begin(), v.end());
    vector<unsigned> vd(2, 1);
    for (unsigned i = 1; i < N; ++i) vd.push_back(v[i] - v[i - 1]);
    sort(vd.begin(), vd.end());
    reverse(vd.begin(), vd.end());
    unsigned l = v[0] - 1, r = K - v.back(),
             k = max(l + r, max(max(l, r) + vd[0] / 2,
                                max(vd[0] / 2 + vd[1] / 2, vd[0] - 1)));
    cout << "Case #" << it << ": " << double(k) / K << endl;
  }
  return 0;
}
