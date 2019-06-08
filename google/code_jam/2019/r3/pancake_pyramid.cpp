#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/treap.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TModular = Modular<>;

namespace {
class PInfo : public bst::info::Size {
 public:
  using TBase = bst::info::Size;
  using TSelf = PInfo;
  static const bool use_data = true;

  TModular s, sp, sl, sr;

  template <class TNode>
  void Update(TNode* node) {
    TBase::Update(node);
    uint64_t size_l = node->l ? node->l->info.size : 0,
             size_r = node->r ? node->r->info.size : 0;
    s = sl = sr = 0;
    sp = node->height;
    if (node->l) {
      s += node->l->info.s;
      s += node->l->info.sl * (size_r + 1);
      sp += node->l->info.sp;
      sl += node->l->info.sl;
      sr += node->l->info.sr;
      sr += (TModular(node->height * size_l) - node->l->info.sp) * (size_r + 1);
    }
    if (node->r) {
      s += node->r->info.s;
      s += node->r->info.sr * (size_l + 1);
      sp += node->r->info.sp;
      sl += node->r->info.sl;
      sr += node->r->info.sr;
      sl += (TModular(node->height * size_r) - node->r->info.sp) * (size_l + 1);
    }
  }
};
}  // namespace

using TTree = bst::Treap<false, false, TEmpty, PInfo>;
using TNode = typename TTree::TNode;

int main_pancake_pyramid() {
  unsigned T, S;
  cin >> T;
  for (unsigned iT = 1; iT <= T; ++iT) {
    cin >> S;
    auto vs = ReadVector<unsigned>(S);
    TTree tree(S);
    vector<TNode*> vn;
    for (unsigned p : vs) {
      TNode* node = tree.New(TEmpty());
      node->height = p;
      vn.push_back(node);
    }
    TNode* head = TTree::BuildTree(vn);
    cout << "Case #" << iT << ": " << head->info.s << endl;
  }
  return 0;
}
