#include "common/stl/base.h"

int main_field_trip() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    vector<int64_t> v(4, numeric_limits<int64_t>::min());
    for (unsigned i = 0; i < 2 * n; ++i) {
      int64_t t;
      cin >> t;
      v[i % 2] = max(v[i % 2], -t);
      v[i % 2 + 2] = max(v[i % 2 + 2], t);
    }
    cout << "Case #" << it << ": " << (max(v[0] + v[2], v[1] + v[3]) + 1) / 2
         << endl;
  }
  return 0;
}
