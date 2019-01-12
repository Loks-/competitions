// https://www.hackerrank.com/challenges/staircase

#include "common/stl/base.h"

int main_staircase() {
  unsigned n;
  cin >> n;
  for (unsigned i = 1; i <= n; ++i) {
    for (unsigned j = i; j < n; ++j) cout << " ";
    for (unsigned j = 0; j < i; ++j) cout << "#";
    cout << endl;
  }
  cout << endl;
  return 0;
}
