// https://leetcode.com/problems/rectangle-overlap/

#include "leetcode/test.h"

#include "common/stl/base.h"

namespace {
class Solution0836 {
 public:
  bool isRectangleOverlap(const vector<int>& rec1, const vector<int>& rec2) {
    return (max(rec1[0], rec2[0]) < min(rec1[2], rec2[2])) &&
           (max(rec1[1], rec2[1]) < min(rec1[3], rec2[3]));
  }
};
}  // namespace

int main_0836() {
  auto Run = [](const pair<vector<int>, vector<int>>& pv) {
    assert((pv.first.size() == 4) && (pv.second.size() == 4));
    Solution0836 s;
    return s.isRectangleOverlap(pv.first, pv.second);
  };

  vector<pair<vector<int>, vector<int>>> input{{{0, 0, 2, 2}, {1, 1, 3, 3}},
                                               {{0, 0, 1, 1}, {1, 0, 2, 1}},
                                               {{0, 0, 1, 1}, {2, 2, 3, 3}}};
  vector<bool> output{true, false, false};
  return TestAll(Run, input, output) ? 0 : -1;
}
