// https://www.hackerrank.com/challenges/divisibility-of-power

#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <functional>

int main_divisibility_of_power() {
  unsigned N, Q;
  cin >> N;
  vector<uint64_t> v = ReadVector<uint64_t>(N);
  cin >> Q;

  std::function<bool(unsigned, unsigned, unsigned)> SolveGE =
      [&](unsigned i, unsigned j, unsigned p) -> bool {
    if ((i > j) || (v[i] == 1)) return (p <= 1);
    if (v[i] == 0) return (p <= 0);
    if (p <= 1) return true;
    unsigned np = 0;
    uint64_t t = 1;
    for (; p > t; t *= v[i]) ++np;
    return SolveGE(i + 1, j, np);
  };

  auto Solve = [&](unsigned i, unsigned j, uint64_t x) {
    if (v[i] == 0) return true;
    unsigned p = 0;
    for (uint64_t g = GCD(v[i], x); g > 1; g = GCD(v[i], x)) {
      p += 1;
      x /= g;
    }
    return (x > 1) ? false : SolveGE(i + 1, j, p);
  };

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    unsigned i, j;
    uint64_t x;
    cin >> i >> j >> x;
    cout << (Solve(i - 1, j - 1, x) ? "Yes" : "No") << endl;
  }
  return 0;
}
