#include "leetcode/test.h"

#include "common/stl/base.h"

namespace {
class Solution0223 {
 public:
  int intersectionArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1,
                       int bx2, int by2) {
    if ((ax1 >= bx2) || (ax2 <= bx1) || (ay1 >= by2) || (ay2 <= by1)) return 0;
    return (min(ax2, bx2) - max(ax1, bx1)) * (min(ay2, by2) - max(ay1, by1));
  }

  int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2,
                  int by2) {
    return (ax2 - ax1) * (ay2 - ay1) + (bx2 - bx1) * (by2 - by1) -
           intersectionArea(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2);
  }
};
}  // namespace

int main_0223() {
  auto Run = [](const vector<int>& v) {
    assert(v.size() == 8);
    Solution0223 s;
    return s.computeArea(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
  };

  vector<vector<int>> input{{-3, 0, 3, 4, 0, -1, 9, 2},
                            {-2, -2, 2, 2, -2, -2, 2, 2}};
  vector<int> output{45, 16};
  return TestAll(Run, input, output) ? 0 : -1;
}
