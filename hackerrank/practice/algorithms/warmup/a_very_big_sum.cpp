// https://www.hackerrank.com/challenges/a-very-big-sum

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <numeric>

int main_a_very_big_sum() {
  unsigned n;
  cin >> n;
  vector<uint64_t> v = nvector::Read<uint64_t>(n);
  cout << accumulate(v.begin(), v.end(), 0ull) << endl;
  return 0;
}
