// https://www.hackerrank.com/challenges/choose-and-calculate

#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TFactorial = modular::mstatic::Factorial<TModularD>;

int main_choose_and_calculate() {
  TFactorial f;
  unsigned N, K;
  cin >> N >> K;
  vector<TModularD> va = nvector::Read<TModularD>(N);
  sort(va.begin(), va.end());
  TModularD s1 = 0, s2 = 0;
  for (unsigned i = 0; i < N; ++i) {
    s1 += va[i] * f.BinomialCoefficient(N - i - 1, K - 1);
    s2 += va[i] * f.BinomialCoefficient(i, K - 1);
  }
  cout << s2 - s1 << endl;
  return 0;
}
