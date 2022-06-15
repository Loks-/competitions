// https://leetcode.com/problems/rectangle-area-ii/

#include "leetcode/test.h"

#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/axis/utils/union_area_rectangles.h"
#include "common/stl/base.h"

namespace {
class Solution0850 {
 public:
  int rectangleArea(const vector<vector<int>>& rectangles) {
    vector<I2ARectangle> vr;
    for (auto& v : rectangles) vr.push_back({{v[0], v[1]}, {v[2], v[3]}});
    return UnionArea(vr) % 1000000007;
  }
};
}  // namespace

int main_0850() {
  auto Run = [](const vector<vector<int>>& v) {
    Solution0850 s;
    return s.rectangleArea(v);
  };

  vector<vector<vector<int>>> input{{{0, 0, 2, 2}, {1, 0, 2, 3}, {1, 0, 3, 1}},
                                    {{0, 0, 1000000000, 1000000000}},
                                    {{0, 0, 2, 2}, {1, 1, 3, 3}}};
  vector<int> output{6, 49, 7};
  return TestAll(Run, input, output) ? 0 : -1;
}
