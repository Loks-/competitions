#pragma once

#include "common/modular/static/modular.h"

#include <vector>

class TesterInterpolation {
 public:
  using TModular = Modular<>;
  static const unsigned power1 = 10, power2 = 100, n = 1000, k = 100;

 protected:
  std::vector<TModular> vp1, vp2;

 public:
  void Init();
  TesterInterpolation();

  static bool TestSumOfPowers(unsigned power, const std::vector<TModular>& vp);
  static bool TestBaseNewtonPolynomial(unsigned power,
                                       const std::vector<TModular>& vp);
  static bool TestAll(unsigned power, const std::vector<TModular>& vp);

  bool TestAll() const;
};
