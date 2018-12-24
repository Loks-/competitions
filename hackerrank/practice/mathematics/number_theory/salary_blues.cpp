// https://www.hackerrank.com/challenges/salary-blues

#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_salary_blues() {
  unsigned N, T;
  cin >> N >> T;
  vector<uint64_t> v = ReadVector<uint64_t>(N);
  uint64_t min_value = v[0], gcd = 0, K;
  for (uint64_t a : v) min_value = min(min_value, a);
  for (uint64_t a : v) gcd = GCD(gcd, a - min_value);
  for (unsigned it = 0; it < T; ++it) {
    cin >> K;
    cout << GCD(gcd, min_value + K) << endl;
  }
  return 0;
}
