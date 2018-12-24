// https://www.hackerrank.com/challenges/kindergarten-adventures

#include "common/binary_indexed_tree/bit.h"
#include "common/stl/base.h"

int main_kindergarten_adventures__bit() {
  unsigned n;
  cin >> n;
  BIT<int> bit(2 * n + 1);
  for (unsigned i = 0; i < n; ++i) {
    unsigned t;
    cin >> t;
    if (t < n) {
      bit.Add(i + 1, 1);
      bit.Add(i + n - t + 1, -1);
    }
  }
  int best = 0;
  unsigned p = 0;
  for (unsigned i = 0; i < n; ++i) {
    int x = bit.Sum(i + 1) + bit.Sum(i + n + 1);
    if (best < x) {
      best = x;
      p = i;
    }
  }
  cout << p + 1 << endl;
  return 0;
}
