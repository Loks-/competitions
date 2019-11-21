// https://www.hackerrank.com/challenges/extremely-dangerous-virus

#include "common/modular_io.h"
#include "common/stl/base.h"

int main_extremely_dangerous_virus() {
  TModularD a, b;
  uint64_t t;
  cin >> a >> b >> t;
  cout << ((a + b) / 2).PowU(t) << endl;
  return 0;
}
