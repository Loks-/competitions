// https://leetcode.com/problems/merge-intervals/

#include "leetcode/test.h"

#include "common/geometry/d1/segment_union_base.h"
#include "common/stl/base.h"

namespace {
class Solution0056 {
 public:
  vector<vector<int>> merge(const vector<vector<int>>& intervals) {
    vector<pair<int, int>> vp;
    for (auto& v : intervals) vp.push_back({v[0], v[1]});
    vector<vector<int>> vv;
    for (auto& p : SegmentMerge(vp)) vv.push_back({p.first, p.second});
    return vv;
  }
};
}  // namespace

int main_0056() {
  auto Run = [](const vector<vector<int>>& vv) {
    Solution0056 s;
    return s.merge(vv);
  };

  vector<vector<vector<int>>> input{{{1, 3}, {2, 6}, {8, 10}, {15, 18}},
                                    {{1, 4}, {4, 5}}};
  vector<vector<vector<int>>> output{{{1, 6}, {8, 10}, {15, 18}}, {{1, 5}}};
  return TestAll(Run, input, output) ? 0 : -1;
}
