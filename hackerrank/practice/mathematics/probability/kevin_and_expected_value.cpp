// https://www.hackerrank.com/challenges/kevin-and-expected-value

#include "common/stl/base.h"

#include <cmath>
#include <iomanip>

int main_kevin_and_expected_value() {
  cout << setprecision(4) << fixed;
  auto F = [](double x) { return (1.0 + sqrt(1.0 + 4.0 * x)) / 2.0; };
  auto FI = [](double x) {
    double t = (1.0 + 4.0 * x);
    return x / 2.0 + t * sqrt(t) / 12.0;
  };

  vector<double> vs(1, 0.);
  uint64_t k, N = 1000000;
  for (unsigned i = 1; i < N; ++i) vs.push_back(vs.back() + F(i));

  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> k;
    cout << ((k <= N) ? vs[k - 1] : vs.back() + FI(k - 0.5) - FI(N - 0.5)) / k
         << endl;
  }
  return 0;
}
