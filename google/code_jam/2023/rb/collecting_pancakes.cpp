#include "common/stl/base.h"
#include "common/vector/read.h"

int main_collecting_pancakes() {
  unsigned T, N, AL, AR, BL, BR;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    vector<uint64_t> v = nvector::Read<uint64_t>(N), vsl(N + 1, 0), vsr(N + 1);
    v.insert(v.begin(), 0u);
    for (unsigned i = 1; i < v.size(); ++i) vsl[i] = vsl[i - 1] + v[i];
    for (unsigned i = 0; i < v.size(); ++i) vsr[i] = vsl.back() - vsl[i] + v[i];
    cin >> AL >> AR >> BL >> BR;
    uint64_t r = 0;
    for (unsigned i = AL; i <= AR; ++i) {
      if (BL >= i) {
        r = max(r, vsl[(i + BL) / 2]);
      } else if (BR <= i) {
        r = max(r, vsr[(i + BR + 1) / 2]);
      } else {
        r = max(r, min(vsl[i], vsr[i]));
      }
    }
    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
