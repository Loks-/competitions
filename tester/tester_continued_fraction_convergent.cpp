#include "common/numeric/continued_fraction/continued_fraction.h"
#include "common/numeric/continued_fraction/semiconvergent.h"
#include "common/numeric/fraction_io.h"
#include "common/vector/write.h"

#include <iostream>
#include <vector>

bool TestContinuedFractionConvergent() {
  std::vector<int64_t> vcf{0, 1, 5, 2, 2};
  numeric::cf::ContinuedFraction cf(vcf);
  std::vector<TIFraction> expected_exact{
      {0, 1}, {1, 1}, {5, 6}, {11, 13}, {27, 32}};
  std::vector<TIFraction> expected_semi{{0, 1}, {1, 1},   {3, 4},   {4, 5},
                                        {5, 6}, {11, 13}, {16, 19}, {27, 32}};
  std::vector<TIFraction> output_exact, output_semi;
  for (numeric::cf::CFConvergent it(cf); !it.End(); ++it)
    output_exact.push_back(*it);
  for (numeric::cf::CFSemiConvergent it(cf); !it.End(); ++it)
    output_semi.push_back(*it);
  bool good = true;
  if (output_exact != expected_exact) {
    std::cout << "CFConvergent output doesn't match expectation:" << std::endl;
    std::cout << "Expected:" << std::endl;
    nvector::Write(expected_exact);
    std::cout << "Output:" << std::endl;
    nvector::Write(output_exact);
    good = false;
  }
  if (output_semi != expected_semi) {
    std::cout << "CFSemiConvergent output doesn't match expectation:"
              << std::endl;
    std::cout << "Expected:" << std::endl;
    nvector::Write(expected_semi);
    std::cout << "Output:" << std::endl;
    nvector::Write(output_semi);
    good = false;
  }
  return good;
}
