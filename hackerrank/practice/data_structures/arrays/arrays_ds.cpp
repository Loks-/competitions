// https://www.hackerrank.com/challenges/arrays-ds

#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/write.h"

int main_arrays_ds() {
  unsigned n;
  cin >> n;
  vector<int> v = ReadVector<int>(n);
  reverse(v.begin(), v.end());
  nvector::Write(v);
  return 0;
}
