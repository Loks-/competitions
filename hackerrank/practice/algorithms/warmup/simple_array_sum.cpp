// https://www.hackerrank.com/challenges/simple-array-sum

#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/sum.h"

int main_simple_array_sum() {
  unsigned n;
  cin >> n;
  cout << nvector::Sum(nvector::Read<unsigned>(n)) << endl;
  return 0;
}
