// https://www.hackerrank.com/challenges/connecting-towns

#include "common/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_C32<1234567>;

int main_connecting_towns() {
  unsigned T, n;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> n;
    TModular x = 1, ni;
    for (unsigned i = 1; i < n; ++i) {
      cin >> ni;
      x *= ni;
    }
    cout << x << endl;
  }
  return 0;
}
