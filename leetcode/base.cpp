#include "common/stl/base.h"

#include "leetcode/test.h"

namespace {
class SolutionBase {
 public:
  int Run(int x) { return x; }
};
}  // namespace

int main_base() {
  auto Run = [](int x) {
    SolutionBase s;
    return s.Run(x);
  };

  vector<int> input{1, 2};
  vector<int> output{1, 2};
  return TestAll(Run, input, output) ? 0 : -1;
}
