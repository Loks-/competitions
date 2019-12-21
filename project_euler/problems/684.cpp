#include "common/modular_io.h"
#include "common/numeric/fibonacci.h"
#include "common/stl/base.h"

int main_684() {
  unsigned N = 90;

  auto S = [](uint64_t n) {
    uint64_t i = (n % 9) + 1;
    return TModularD(10).PowU(n / 9) * ((i * (i + 1)) / 2 + 5) - (n + 6);
  };

  TModularD r;
  numeric::Fibonacci f;
  for (unsigned i = 2; i <= N; ++i) {
    r += S(f(i));
  }
  std::cout << r << std::endl;
  return 0;
}
