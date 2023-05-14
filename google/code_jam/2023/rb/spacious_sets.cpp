#include "common/stl/base.h"
#include "common/vector/read.h"

int main_spacious_sets() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    vector<int64_t> v = nvector::Read<int64_t>(N), vs = v;
    sort(vs.begin(), vs.end());
    vector<unsigned> v1, v2;
    for (unsigned j = 0; j < 2; ++j) {
      auto& vj = (j ? v2 : v1);
      for (auto x : vs) {
        auto k = upper_bound(vs.begin(), vs.end(), x - K) - vs.begin();
        vj.push_back(1u + (k ? vj[k - 1] : 0u));
      }
      reverse(vs.begin(), vs.end());
      for (auto& x : vs) x = -x;
    }
    cout << "Case #" << it << ":";
    for (auto x : v) {
      auto k = lower_bound(vs.begin(), vs.end(), x) - vs.begin();
      cout << " " << v1[k] + v2[N - k - 1] - 1;
    }
    cout << endl;
  }
  return 0;
}
