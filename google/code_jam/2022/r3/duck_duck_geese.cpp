#include "common/data_structures/segment_tree/action/add_each__max.h"
#include "common/data_structures/segment_tree/base/add_action_to_segment.h"
#include "common/data_structures/segment_tree/base/get_segment_info.h"
#include "common/data_structures/segment_tree/info/max_count.h"
#include "common/data_structures/segment_tree/segment_tree.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TSTree = ds::st::SegmentTree<int, ds::st::info::MaxCount<int>,
                                   ds::st::action::AddEachMax<int>>;

int main_duck_duck_geese() {
  unsigned T, N, C;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> C;
    vector<unsigned> vc1(C, 0), vc2(C, 0);
    for (unsigned i = 0; i < C; ++i) {
      cin >> vc1[i] >> vc2[i];
      if (vc1[i] == 0) ++vc1[i];
      ++vc2[i];
    }
    auto v = nvector::Read<unsigned>(N);
    for (auto& x : v) --x;
    v.insert(v.end(), v.begin(), v.end());
    vector<vector<unsigned>> vcp(C, {0});
    for (unsigned i = 0; i < v.size(); ++i) vcp[v[i]].push_back(i);

    auto Get = [&](unsigned c, unsigned ind) {
      return (ind < vcp[c].size()) ? vcp[c][ind] : unsigned(v.size());
    };

    TSTree tree(v.size());
    auto root = tree.BuildTree(vector<int>(v.size(), 0));
    for (unsigned c = 0; c < C; ++c) {
      ds::st::AddActionToSegment(root, Get(c, 0), Get(c, 1), 1);
      ds::st::AddActionToSegment(root, Get(c, vc1[c]), Get(c, vc2[c]), 1);
    }
    uint64_t total = 0;
    vector<unsigned> vc(C, 0);
    for (unsigned i = 0; i < N; ++i) {
      auto si = ds::st::GetSegmentInfo(root, i + 1, i + N - 1);
      if (si.max == int(C)) total += si.max_count;
      auto c = v[i];
      ++vc[c];
      ds::st::AddActionToSegment(root, Get(c, vc[c]), Get(c, vc[c] + 1), 1);
      ds::st::AddActionToSegment(root, Get(c, vc[c] + vc1[c] - 1),
                                 Get(c, vc[c] + vc1[c]), -1);
      ds::st::AddActionToSegment(root, Get(c, vc[c] + vc2[c] - 1),
                                 Get(c, vc[c] + vc2[c]), 1);
    }
    cout << "Case #" << it << ": " << total << endl;
  }
  return 0;
}
