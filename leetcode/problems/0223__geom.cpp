#include "leetcode/test.h"

#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/axis/utils/union_area_2rectangles.h"
#include "common/stl/base.h"

namespace {
class Solution0223Geom {
 public:
  int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2,
                  int by2) {
    I2ARectangle r1({ax1, ay1}, {ax2, ay2}), r2({bx1, by1}, {bx2, by2});
    return UnionArea(r1, r2);
  }
};
}  // namespace

int main_0223__geom() {
  auto Run = [](const vector<int>& v) {
    assert(v.size() == 8);
    Solution0223Geom s;
    return s.computeArea(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
  };

  vector<vector<int>> input{{-3, 0, 3, 4, 0, -1, 9, 2},
                            {-2, -2, 2, 2, -2, -2, 2, 2}};
  vector<int> output{45, 16};
  return TestAll(Run, input, output) ? 0 : -1;
}
