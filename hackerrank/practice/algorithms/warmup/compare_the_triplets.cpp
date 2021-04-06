// https://www.hackerrank.com/challenges/compare-the-triplets

#include "common/stl/base.h"
#include "common/vector/read.h"

int main_compare_the_triplets() {
  vector<unsigned> v1 = nvector::Read<unsigned>(3),
                   v2 = nvector::Read<unsigned>(3);
  unsigned a = 0, b = 0;
  for (unsigned i = 0; i < 3; ++i) {
    if (v1[i] < v2[i])
      ++b;
    else if (v1[i] > v2[i])
      ++a;
  }
  cout << a << " " << b << endl;
  return 0;
}
