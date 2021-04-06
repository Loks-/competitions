// https://www.hackerrank.com/challenges/simple-array-sum

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <numeric>

int main_mini_max_sum() {
  vector<unsigned> v = nvector::Read<unsigned>(5);
  unsigned s = accumulate(v.begin(), v.end(), 0u);
  cout << s - *max_element(v.begin(), v.end()) << " "
       << s - *min_element(v.begin(), v.end()) << endl;
  return 0;
}
