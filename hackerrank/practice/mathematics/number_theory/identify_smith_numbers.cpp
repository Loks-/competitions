// https://www.hackerrank.com/challenges/identify-smith-numbers

#include "common/factorization/utils/factorization_base.h"
#include "common/numeric/utils/digits_sum.h"
#include "common/stl/base.h"

int main_identify_smith_numbers() {
  uint64_t n;
  cin >> n;
  uint64_t s = 0;
  TFactorization vf = FactorizeBase(n);
  for (auto p : vf) s += DigitsSum(p.prime) * p.power;
  cout << (s == DigitsSum(n) ? 1 : 0) << endl;
  return 0;
}
