// https://www.hackerrank.com/challenges/plus-minus

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <iomanip>

int main_plus_minus() {
  cout << setprecision(6) << fixed;
  unsigned n;
  cin >> n;
  vector<int> v = ReadVector<int>(n);
  vector<unsigned> vc(3, 0);
  for (int x : v) {
    if (x > 0)
      ++vc[0];
    else if (x < 0)
      ++vc[1];
    else
      ++vc[2];
  }
  for (unsigned c : vc) cout << double(c) / n << endl;
  return 0;
}
