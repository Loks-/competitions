// https://www.hackerrank.com/challenges/time-conversion

#include "common/stl/base.h"

int main_time_conversion() {
  string time;
  cin >> time;
  int h = 10 * time[0] + time[1] - 11 * '0';
  if (h == 12) h -= 12;
  if (time[8] == 'P') h += 12;
  cout << (h / 10) << h % 10 << time.substr(2, 6) << endl;
  return 0;
}
