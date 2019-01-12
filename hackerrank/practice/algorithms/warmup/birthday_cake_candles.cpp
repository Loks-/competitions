// https://www.hackerrank.com/challenges/birthday-cake-candles

#include "common/stl/base.h"
#include "common/vector/read.h"

int main_birthday_cake_candles() {
  unsigned n;
  cin >> n;
  vector<unsigned> v = ReadVector<unsigned>(n);
  unsigned value = *max_element(v.begin(), v.end());
  cout << count(v.begin(), v.end(), value) << endl;
  return 0;
}
