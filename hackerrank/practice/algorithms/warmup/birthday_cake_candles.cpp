// https://www.hackerrank.com/challenges/birthday-cake-candles

#include "common/stl/base.h"
#include "common/vector/max.h"
#include "common/vector/read.h"

int main_birthday_cake_candles() {
  unsigned n;
  cin >> n;
  vector<unsigned> v = nvector::Read<unsigned>(n);
  cout << count(v.begin(), v.end(), nvector::Max(v)) << endl;
  return 0;
}
