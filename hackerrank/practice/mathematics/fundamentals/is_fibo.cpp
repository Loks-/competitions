// https://www.hackerrank.com/challenges/is-fibo

#include "common/stl/base.h"

int main_is_fibo() {
  vector<uint64_t> vf{0, 1};
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N;
    cin >> N;
    for (; vf.back() <= N;) vf.push_back(vf[vf.size() - 2] + vf[vf.size() - 1]);
    cout << (binary_search(vf.begin(), vf.end(), N) ? "IsFibo" : "IsNotFibo")
         << endl;
  }
  return 0;
}
