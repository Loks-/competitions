#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/sum.h"
#include "common/vector/write.h"

int main_equal_sum() {
  uint64_t M = PowU<uint64_t>(10, 9);
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    vector<uint64_t> v1(1, 1);
    for (; v1.back() * 2 <= M;) v1.push_back(v1.back() * 2);
    if (v1.size() > N) v1.resize(N);
    for (; v1.size() < N;) v1.push_back(v1.back() + 1);
    nvector::Write(v1);
    auto v2 = nvector::Read<uint64_t>(N);
    v1.insert(v1.end(), v2.begin(), v2.end());
    sort(v1.begin(), v1.end());
    reverse(v1.begin(), v1.end());
    auto S = nvector::Sum(v1) / 2;
    for (auto x : v1) {
      if (x <= S) {
        cout << x << " ";
        S -= x;
      }
    }
    cout << endl;
  }
  return 0;
}
