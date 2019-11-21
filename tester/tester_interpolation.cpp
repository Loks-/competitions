#include "tester/tester_interpolation.h"

#include "common/modular/static/modular.h"
#include "common/modular/static/sum_of_powers.h"
#include "common/polynomial/base_newton.h"

#include <iostream>
#include <vector>

void TesterInterpolation::Init() {
  vp1.resize(n);
  vp2.resize(n);
  vp1[0] = vp2[0] = TModular(0);
  for (unsigned i = 1; i < n; ++i) {
    vp1[i] = vp1[i - 1] + TModular(i).PowU(power1);
    vp2[i] = vp2[i - 1] + TModular(i).PowU(power2);
  }
}

TesterInterpolation::TesterInterpolation() { Init(); }

bool TesterInterpolation::TestSumOfPowers(unsigned power,
                                          const std::vector<TModular>& vp) {
  ModularSumOfPowers<TModular> s;
  for (unsigned i = 0; i < n; ++i) {
    if (s.Sum(i, power) != vp[i]) {
      std::cout << "TestSumOfPowers failed:"
                << "\n\tpower = " << power << "\tindex = " << i
                << "\n\tvalue = " << s.Sum(i, power).Get()
                << "\texpected = " << vp[i].Get() << std::endl;
      return false;
    }
  }
  return true;
}

bool TesterInterpolation::TestBaseNewtonPolynomial(
    unsigned power, const std::vector<TModular>& vp) {
  std::vector<TModular> vtemp(vp.begin(), vp.begin() + power + 2);
  polynomial::BaseNewton<TModular> p;
  p.Interpolate(vtemp);
  for (unsigned i = n - k; i < n; ++i) {
    if (p(TModular(i)) != vp[i]) {
      std::cout << "TestBaseNewtonPolynomial failed:\n"
                << "\tpower = " << power << "\tindex = " << i << std::endl;
      return false;
    }
  }
  return true;
}

bool TesterInterpolation::TestAll(unsigned power,
                                  const std::vector<TModular>& vp) {
  bool b = true;
  b = TestSumOfPowers(power, vp) && b;
  b = TestBaseNewtonPolynomial(power, vp) && b;
  return b;
}

bool TesterInterpolation::TestAll() const {
  return TestAll(power1, vp1) && TestAll(power2, vp2);
}

bool TestInterpolation() {
  TesterInterpolation tester;
  return tester.TestAll();
}
