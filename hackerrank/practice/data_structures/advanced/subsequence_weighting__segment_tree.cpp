// https://www.hackerrank.com/challenges/subsequence-weighting

#include "common/segment_tree/base/find_leaf.h"
#include "common/segment_tree/base/get_segment_info.h"
#include "common/segment_tree/info/max.h"
#include "common/segment_tree/info/update_node_to_root.h"
#include "common/segment_tree/segment_tree.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <set>

using TTree =
    st::SegmentTree<uint64_t,
                    st::info::Max<uint64_t, st::info::Position<uint64_t>>>;
using TNode = TTree::TNode;
using TInfo = TNode::TInfo;

int main_subsequence_weighting__segment_tree() {
  unsigned T, N;
  cin >> T;
  TTree tree(150000);
  for (unsigned it = 0; it < T; ++it) {
    tree.ResetNodes();
    cin >> N;
    vector<uint64_t> va = ReadVector<uint64_t>(N), vw = ReadVector<uint64_t>(N);
    set<uint64_t> s(va.begin(), va.end());
    vector<uint64_t> vaus(s.begin(), s.end());
    TNode* root = tree.BuildTree(vector<uint64_t>(vaus.size(), 0), vaus);
    for (unsigned i = 0; i < N; ++i) {
      TInfo info = st::GetSegmentInfo<TNode>(root, 0, va[i] - 1);
      TNode* node = st::FindLeaf(root, va[i]);
      assert(node && node->IsLeaf());
      node->GetData() = std::max(node->GetData(), info.segment_max + vw[i]);
      st::info::UpdateNodeToRoot<TNode>(node);
    }
    cout << root->info.segment_max << endl;
  }
  return 0;
}
