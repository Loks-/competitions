// https://www.hackerrank.com/challenges/fibonacci-gcd

#include "common/modular/static/fibonacci_matrix.h"
#include "common/modular/static/modular_io.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

using TFibonacci = ModularFibonacciMatrix<Modular<1000000007>>;

int main_fibonacci_gcd() {
  unsigned N;
  cin >> N;
  uint64_t a, b;
  cin >> a;
  for (unsigned i = 1; i < N; ++i) {
    cin >> b;
    a = GCD(b, a);
  }
  cout << TFibonacci::GetU(a) << endl;
  return 0;
}
