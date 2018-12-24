// https://www.hackerrank.com/challenges/tile-painting-revisited

#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_tile_painting_revisited() {
  unsigned T;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    uint64_t N;
    cin >> N;
    TModular s = N;
    for (uint64_t l = 1; l * l < N; ++l) {
      s += (N - l * l);
      uint64_t k = (N - 1) / l;
      s += TModular(k - l) * (2 * N - l * (k + l + 1));
    }
    cout << s << endl;
  }
  return 0;
}
