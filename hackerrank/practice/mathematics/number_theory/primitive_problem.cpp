// https://www.hackerrank.com/challenges/primitive-problem

#include "common/factorization/utils/eulers_totient.h"
#include "common/factorization/utils/factorization_base.h"
#include "common/modular/utils/primitive_root.h"
#include "common/stl/base.h"

int main_primitive_problem() {
  uint64_t p;
  cin >> p;
  TFactorization p1f = FactorizeBase(p - 1);
  uint64_t u = FindSmallestPrimitive(p, p1f);
  cout << u << " " << EulersTotient(p - 1, p1f) << endl;
  return 0;
}
