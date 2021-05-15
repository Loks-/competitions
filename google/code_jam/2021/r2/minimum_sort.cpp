#include "common/stl/base.h"

int main_minimum_sort() {
  unsigned T, N, j;
  cin >> T >> N;
  for (unsigned it = 1; it <= T; ++it) {
    for (unsigned i = 1; i < N; ++i) {
      cout << "M " << i << " " << N << endl;
      cin >> j;
      if (i != j) {
        cout << "S " << i << " " << j << endl;
        cin >> j;
      }
    }
    cout << "D" << endl;
    cin >> j;
  }
  return 0;
}
