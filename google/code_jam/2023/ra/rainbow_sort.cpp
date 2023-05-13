#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/write.h"

#include <unordered_set>

int main_rainbow_sort() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    vector<unsigned> v = nvector::Read<unsigned>(N), v2(1, v[0]);
    unordered_set<unsigned> s{v2.begin(), v2.end()};
    bool ok = true;
    for (unsigned i = 1; i < N; ++i) {
      if (v[i] == v[i - 1]) continue;
      if (s.find(v[i]) != s.end()) ok = false;
      v2.push_back(v[i]);
      s.insert(v[i]);
    }
    cout << "Case #" << it << ": ";
    if (ok) {
      nvector::Write(v2);
    } else {
      cout << "IMPOSSIBLE" << endl;
    }
  }
  return 0;
}
