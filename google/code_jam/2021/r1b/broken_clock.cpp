#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

static const uint64_t P = 43200000000000ull;
using TModular = modular::mstatic::Modular<P, false, false>;

int main_broken_clock() {
  TModular a = TModular(1) / TModular(11), b = 0;
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    auto v = nvector::Read<TModular>(3);
    for (;; next_permutation(v.begin(), v.end())) {
      auto h = v[0], m = v[1], s = v[2];
      b = a * (h * TModular(12) - m);
      if (s == b + TModular(720) * (h - b)) break;
    }
    cout << "Case #" << it << ": " << (v[0] - b).Get() / (P / 12) << " "
         << (v[1] - b).Get() / (P / 60) << " " << (v[2] - b).Get() / (P / 60)
         << " " << (v[0] - b).Get() % 1000000000ull << endl;
  }
  return 0;
}
