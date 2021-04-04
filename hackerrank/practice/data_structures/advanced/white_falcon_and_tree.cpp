// https://www.hackerrank.com/challenges/white-falcon-and-tree

#include "common/graph/tree.h"
#include "common/graph/tree/hld.h"
#include "common/linear_algebra/matrix_static_size.h"
#include "common/modular_io.h"
#include "common/segment_tree/action/none.h"
#include "common/segment_tree/info/none.h"
#include "common/stl/base.h"

namespace {
class FData {
 public:
  TModularD a, b, br;
};

class FInfo : public st::info::None {
 public:
  using TBase = st::info::None;
  using TSelf = FInfo;

  static const bool is_none = false;
  static const bool use_data = true;

  FData f;

  template <class TData, class TSInfo>
  void UpdateData(const TData& data, const TSInfo& sinfo) {
    TBase::UpdateData(data, sinfo);
    f = data;
  }

  template <class TSInfo>
  void UpdateLR(const TSelf& l, const TSelf& r, const TSInfo& sinfo) {
    TBase::UpdateLR(l, r, sinfo);
    f.a = l.f.a * r.f.a;
    f.b = r.f.a * l.f.b + r.f.b;
    f.br = l.f.a * r.f.br + l.f.br;
  }
};

class FActionSet : public st::action::None {
 public:
  static const bool modify_data = true;

  bool empty;
  FData f;

  FActionSet() : empty(true) {}

  bool IsEmpty() const { return empty; }
  void Clear() { empty = true; };

  template <class TNode>
  void Add(TNode* node, const FData& value) {
    assert(value.b == value.br);
    empty = false;
    f = value;
    auto m = (SquareMatrixStaticSize<TModularD, 2>{f.a, f.b, 0, 1})
                 .PowU(node->sinfo.Size());
    node->info.f.a = m(0, 0);
    node->info.f.b = m(0, 1);
    node->info.f.br = m(0, 1);
  }

  template <class TNode>
  void Apply(TNode* node) {
    if (IsEmpty()) return;
    if (node->IsLeaf()) {
      node->GetData() = f;
    } else {
      node->l->AddAction(f);
      node->r->AddAction(f);
    }
    Clear();
  }
};
}  // namespace

int main_white_falcon_and_tree() {
  unsigned N, Q;
  cin >> N;
  vector<FData> v(N);
  for (unsigned i = 0; i < N; ++i) {
    cin >> v[i].a >> v[i].b;
    v[i].br = v[i].b;
  }
  TreeGraph tree(N);
  tree.ReadEdges();
  graph::HLD<FData, FInfo, FActionSet> hld(tree, v);

  cin >> Q;
  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    unsigned t, u, v, a, b;
    cin >> t >> u >> v >> a;
    if (t == 1) {
      cin >> b;
      hld.Path(u - 1, v - 1).AddAction(FData{a, b, b});
    } else if (t == 2) {
      u -= 1;
      v -= 1;
      unsigned c = hld.LCA(u, v);
      auto f1 = hld.PathFromAncestor(c, u, false).GetInfo();
      auto y = f1.f.a * a + f1.f.br;
      if (v != c) {
        auto f2 = hld.PathFromAncestor(c, v, true).GetInfo();
        y = f2.f.a * y + f2.f.b;
      }
      cout << y << endl;
    }
  }
  return 0;
}
