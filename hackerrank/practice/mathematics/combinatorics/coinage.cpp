// https://www.hackerrank.com/challenges/coinage

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <functional>

int main_coinage() {
  unsigned T, n;
  vector<unsigned> vs, vc{1, 2, 5, 10};

  std::function<uint64_t(unsigned, unsigned)> SolveR =
      [&](unsigned k, unsigned n) -> uint64_t {
    if (k == 0) return (n <= vs[0]) ? 1 : 0;
    uint64_t r = 0;
    for (unsigned i = 0; (i <= vs[k]) && (i * vc[k] <= n); ++i)
      r += SolveR(k - 1, n - i * vc[k]);
    return r;
  };

  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> n;
    vs = ReadVector<unsigned>(4);
    cout << SolveR(3, n) << endl;
  }
  return 0;
}
