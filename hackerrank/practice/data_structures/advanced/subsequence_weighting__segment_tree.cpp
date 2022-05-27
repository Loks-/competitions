// https://www.hackerrank.com/challenges/subsequence-weighting

#include "common/data_structures/segment_tree/base/find_leaf.h"
#include "common/data_structures/segment_tree/base/get_segment_info.h"
#include "common/data_structures/segment_tree/info/max.h"
#include "common/data_structures/segment_tree/info/update_node_to_root.h"
#include "common/data_structures/segment_tree/segment_tree.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <set>

using TTree = ds::st::SegmentTree<uint64_t, ds::st::info::Max<uint64_t>,
                                  ds::st::action::None,
                                  ds::st::sinfo::Position<uint64_t>>;
using TNode = TTree::TNode;
using TInfo = TNode::TInfo;

int main_subsequence_weighting__segment_tree() {
  unsigned T, N;
  cin >> T;
  TTree tree(150000);
  for (unsigned it = 0; it < T; ++it) {
    tree.ResetNodes();
    cin >> N;
    vector<uint64_t> va = nvector::Read<uint64_t>(N),
                     vw = nvector::Read<uint64_t>(N);
    set<uint64_t> s(va.begin(), va.end());
    vector<uint64_t> vaus(s.begin(), s.end());
    vaus.push_back(vaus.back() + 1);
    TNode* root = tree.BuildTree(vector<uint64_t>(s.size(), 0), vaus);
    for (unsigned i = 0; i < N; ++i) {
      TInfo info = ds::st::GetSegmentInfo<TNode>(root, 0, va[i]);
      TNode* node = ds::st::FindLeaf(root, va[i]);
      assert(node && node->IsLeaf());
      node->GetData() = std::max(node->GetData(), info.max + vw[i]);
      ds::st::info::UpdateNodeToRoot<TNode>(node);
    }
    cout << root->info.max << endl;
  }
  return 0;
}
