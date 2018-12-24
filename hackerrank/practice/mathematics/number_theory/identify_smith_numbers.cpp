// https://www.hackerrank.com/challenges/identify-smith-numbers

#include "common/factorization/utils/factorize.h"
#include "common/numeric/utils/digits_sum.h"
#include "common/stl/base.h"

int main_identify_smith_numbers() {
  uint64_t n;
  cin >> n;
  uint64_t s = 0;
  TFactorization vf = Factorize(n);
  for (auto p : vf) s += DigitsSum(p.first) * p.second;
  cout << (s == DigitsSum(n) ? 1 : 0) << endl;
  return 0;
}
