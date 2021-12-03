#include "common/stl/base.h"
#include "common/vector/read.h"

int main_2003b() {
  unsigned n;
  cin >> n;
  auto v = nvector::Read<string>(n);
  unsigned l = v[0].size(), m = 1;
  auto Solve = [&](unsigned dx, unsigned dy) {
    unsigned s = 0;
    for (unsigned i = 0; i < n; i += dy) {
      s += (v[i][(dx * (i / dy)) % l] == '#') ? 1 : 0;
    }
    return s;
  };

  m *= Solve(1, 1);
  m *= Solve(3, 1);
  m *= Solve(5, 1);
  m *= Solve(7, 1);
  m *= Solve(1, 2);
  cout << m << endl;
  return 0;
}
