// https://www.hackerrank.com/challenges/cheese-and-random-toppings

#include "common/modular/proxy/binomial_coefficient.h"
#include "common/stl/base.h"

int main_cheese_and_random_toppings__bcm() {
  unsigned T, N, R, M;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> R >> M;
    modular::proxy::BinomialCoefficient bcm(M);
    cout << bcm.Apply(N, R) << endl;
  }
  return 0;
}
