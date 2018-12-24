// https://www.hackerrank.com/challenges/help-mike

#include "common/stl/base.h"

int main_help_mike() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N, K, s = 0;
    cin >> N >> K;
    for (uint64_t i = 1; i <= K; ++i) {
      if ((i == K) || (2 * i == K))
        s += ((N + K - i) / K) * ((N - i) / K);
      else
        s += ((N + K - i) / K) * ((N + i) / K);
    }
    cout << s / 2 << endl;
  }
  return 0;
}
