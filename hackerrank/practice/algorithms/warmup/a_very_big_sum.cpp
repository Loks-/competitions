// https://www.hackerrank.com/challenges/a-very-big-sum

#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/sum.h"

int main_a_very_big_sum() {
  unsigned n;
  cin >> n;
  cout << nvector::Sum(nvector::Read<uint64_t>(n)) << endl;
  return 0;
}
