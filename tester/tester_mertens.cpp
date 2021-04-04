#include "common/factorization/mertens.h"
#include "common/vector/write.h"

#include <iostream>
#include <vector>

bool TestMertens() {
  factorization::Mertens mertens(1ull << 20);
  std::vector<int> expected{1,   0,     -1,  -2,   -1,    -4,   -1,    -2,
                            -1,  -4,    -4,  7,    -19,   22,   -32,   26,
                            14,  -20,   24,  -125, 257,   -362, 228,   -10,
                            211, -1042, 329, 330,  -1703, 6222, -10374};
  std::vector<int> output;
  for (unsigned i = 0; i <= 30; ++i) output.push_back(mertens(1ull << i));
  if (output != expected) {
    std::cout << "Expected:" << std::endl;
    nvector::Write(expected);
    std::cout << "Output:" << std::endl;
    nvector::Write(output);
    return false;
  }
  return true;
}
