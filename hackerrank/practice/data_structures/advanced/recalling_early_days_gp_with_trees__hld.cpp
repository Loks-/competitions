// https://www.hackerrank.com/challenges/recalling-early-days-gp-with-trees

#include "common/data_structures/segment_tree/action/none.h"
#include "common/data_structures/segment_tree/info/sum.h"
#include "common/graph/tree.h"
#include "common/graph/tree/hld.h"
#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/stl/pair.h"

using TModular = TModular_P32<100711433>;

namespace {
class FData {
 public:
  TModular f0, f1, v;

  FData operator+(const FData& r) const {
    return FData{f0 + r.f0, f1 + r.f1, v + r.v};
  }
};

class ActionAdd : public ds::st::action::None {
 public:
  using TData = std::pair<TModular, TModular>;
  static const bool modify_data = true;

  TData x;

  bool IsEmpty() const { return (x.first == 0) && (x.second == 0); }
  void Clear() { x = {0, 0}; };

  template <class TNode>
  void Add(TNode* node, const TData& value) {
    auto& i = node->info.sum;
    i.v += i.f0 * value.first + i.f1 * value.second;
    x += value;
  }

  template <class TNode>
  void Apply(TNode* node) {
    if (IsEmpty()) return;
    if (node->IsLeaf()) {
      auto& d = node->GetData();
      d.v += d.f0 * x.first + d.f1 * x.second;
    } else {
      node->l->AddAction(x);
      node->r->AddAction(x);
    }
    Clear();
  }
};
}  // namespace

int main_recalling_early_days_gp_with_trees__hld() {
  unsigned N, R, U, Q;
  cin >> N >> R;
  TreeGraph tree(N);
  tree.ReadEdges();
  graph::HLD<FData, ds::st::info::Sum<FData>, ActionAdd> hld(tree);

  TModular r(R), ri = (r ? r.Inverse() : 0);
  for (unsigned i = 0; i < N; ++i) {
    auto d = hld.Deep(i);
    hld.SetData(i, FData{r.PowU(d), ri.PowU(d), 0});
  }

  cin >> U >> Q;
  unsigned a, b;
  TModular x;
  for (unsigned iU = 0; iU < U; ++iU) {
    cin >> a >> b >> x;
    a -= 1;
    b -= 1;
    if (r) {
      unsigned c = hld.LCA(a, b), d = hld.Deep(c);
      auto y = x * r.PowU(hld.DistanceFromAncestor(c, a));
      hld.PathFromAncestor(c, a, false)
          .AddAction(ActionAdd::TData{0, y * r.PowU(d)});
      if (b != c)
        hld.PathFromAncestor(c, b, true)
            .AddAction(ActionAdd::TData{y * ri.PowU(d), 0});
    } else {
      hld.SetData(a, hld.Node(a)->GetData() + FData{0, 0, x});
    }
  }
  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    cin >> a >> b;
    cout << hld.Path(a - 1, b - 1).GetInfo().sum.v << endl;
  }
  return 0;
}
