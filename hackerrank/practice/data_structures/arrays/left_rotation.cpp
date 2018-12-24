// https://www.hackerrank.com/challenges/array-left-rotation/

#include "common/stl/base.h"
#include "common/vector/write.h"

int main_left_rotation() {
  unsigned n, d;
  cin >> n >> d;
  vector<unsigned> v(n);
  for (unsigned i = 0; i < n; ++i) cin >> v[(i + n - d) % n];
  WriteVector(v);
  return 0;
}
