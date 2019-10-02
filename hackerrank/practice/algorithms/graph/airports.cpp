// https://www.hackerrank.com/challenges/airports

#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/utils/segment_info_by_key.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

namespace {
class NodeInfoAirports : public bst::info::Size {
 public:
  using TBase = bst::info::Size;
  static const bool is_none = false;

  int64_t l, r, d;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    l = (node->l) ? node->l->info.l : node->key;
    r = (node->r) ? node->r->info.r : node->key;
    d = 0;
    if (node->l)
      d = std::max(d, std::max(node->l->info.d, node->key - node->l->info.r));
    if (node->r)
      d = std::max(d, std::max(node->r->info.d, node->r->info.l - node->key));
  }
};
}  // namespace

using TTree = bst::Treap<true, false, TEmpty, NodeInfoAirports>;
using TNode = TTree::TNode;

int main_airports() {
  TEmpty e;
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned n;
    int64_t d;
    cin >> n >> d;
    auto v = ReadVector<int64_t>(n);
    int64_t l = min(v[0], v[1]), r = max(v[0], v[1]);
    TTree tree(n);
    cout << 0 << " " << max<int64_t>(d - (r - l), 0);
    TNode* head = nullptr;
    typename TNode::TInfo info;
    for (unsigned i = 2; i < n; ++i) {
      if (v[i] < l) {
        head = tree.InsertNewNode(head, e, l);
        l = v[i];
      } else if (r < v[i]) {
        head = tree.InsertNewNode(head, e, r);
        r = v[i];
      } else {
        head = tree.InsertNewNode(head, e, v[i]);
      }
      info.size = 0;
      head = bst::SegmentInfoByKey<TTree>(head, r - d + 1, l + d, info);
      int64_t lrd = max<int64_t>(d - (r - l), 0);
      if (info.size == 0) {
        cout << " " << lrd;
      } else {
        int64_t md = max(info.l - (r - d), max(info.d, (l + d) - info.r));
        cout << " " << max(lrd, l - r + 2 * d - md);
      }
    }
    cout << endl;
  }
  return 0;
}
