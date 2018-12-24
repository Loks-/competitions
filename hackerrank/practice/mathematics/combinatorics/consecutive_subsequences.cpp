// https://www.hackerrank.com/challenges/consecutive-subsequences

#include "common/stl/base.h"
#include "common/vector/read.h"

int main_consecutive_subsequences() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> N >> K;
    vector<unsigned> va = ReadVector<unsigned>(N);
    vector<uint64_t> vc(K, 0);
    vc[0] = 1;
    unsigned s = 0;
    for (unsigned i = 0; i < N; ++i) {
      s = (s + va[i]) % K;
      vc[s] += 1;
    }
    uint64_t r = 0;
    for (uint64_t c : vc) r += (c * (c - 1)) / 2;
    cout << r << endl;
  }
  return 0;
}
