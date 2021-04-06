// https://www.hackerrank.com/challenges/vertical-sticks

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <functional>
#include <iomanip>

int main_vertical_sticks() {
  std::function<double(unsigned, unsigned)> Get = [&](unsigned l,
                                                      unsigned n) -> double {
    return (l == 0) ? 1.0 : 1.0 + (1.0 * l) / n * Get(l - 1, n - 1);
  };

  cout << setprecision(2) << fixed;
  unsigned T;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    unsigned n;
    cin >> n;
    vector<unsigned> v = nvector::Read<unsigned>(n);
    sort(v.begin(), v.end());
    double sum = 0;
    unsigned j = 0;
    for (unsigned i = 0; i < n; ++i) {
      for (; v[j] < v[i];) ++j;
      sum += Get(j, n);
    }
    cout << sum << endl;
  }
  return 0;
}
