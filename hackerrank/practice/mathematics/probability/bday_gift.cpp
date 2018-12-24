// https://www.hackerrank.com/challenges/bday-gift

#include "common/stl/base.h"

#include <iomanip>

int main_bday_gift() {
  cout << setprecision(1) << fixed;
  unsigned T, n;
  cin >> T;
  double sum = 0.;
  for (unsigned it = 0; it < T; ++it) {
    cin >> n;
    sum += n;
  }
  cout << sum / 2.0 << endl;
  return 0;
}
