// https://www.hackerrank.com/challenges/simple-array-sum

#include "common/stl/base.h"
#include "common/vector/max.h"
#include "common/vector/min.h"
#include "common/vector/read.h"
#include "common/vector/sum.h"

int main_mini_max_sum() {
  vector<unsigned> v = nvector::Read<unsigned>(5);
  unsigned s = nvector::Sum(v);
  cout << s - nvector::Max(v) << " " << s - nvector::Min(v) << endl;
  return 0;
}
