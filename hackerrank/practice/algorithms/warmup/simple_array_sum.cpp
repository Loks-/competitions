// https://www.hackerrank.com/challenges/simple-array-sum

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <numeric>

int main_simple_array_sum() {
  unsigned n;
  cin >> n;
  vector<unsigned> v = nvector::Read<unsigned>(n);
  cout << accumulate(v.begin(), v.end(), 0u) << endl;
  return 0;
}
