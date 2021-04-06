#include "common/stl/base.h"
#include "common/vector/read.h"

// Time: O(T * N^2)
// Memory: O(N)
int main_018() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    auto vl = nvector::Read<unsigned>(1);
    for (unsigned i = 2; i <= N; ++i) {
      auto vc = nvector::Read<unsigned>(i);
      vc[0] += vl[0];
      vc.back() += vl.back();
      for (unsigned j = 1; j < vl.size(); ++j)
        vc[j] += std::max(vl[j - 1], vl[j]);
      vl.swap(vc);
    }
    cout << *max_element(vl.begin(), vl.end()) << endl;
  }
  return 0;
}
