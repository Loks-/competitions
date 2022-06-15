#include "common/data_structures/segment_tree/action/add_each__s_dot_product.h"
#include "common/data_structures/segment_tree/action/apply_root_to_node.h"
#include "common/data_structures/segment_tree/info/sum_first.h"
#include "common/data_structures/segment_tree/info/update_node_to_root.h"
#include "common/data_structures/segment_tree/sinfo/sum_second.h"
#include "common/data_structures/segment_tree/sinfo/update_tree.h"
#include "common/graph/tree/hld.h"
#include "common/linear_algebra/vector_static_size.h"

#include <utility>

namespace graph {
template <class TTValue, unsigned _vsize>
class HLDSDP
    : public HLD<std::pair<TTValue, la::VectorStaticSize<TTValue, _vsize>>,
                 ds::st::info::SumFirst<TTValue>,
                 ds::st::action::AddEachSDotProduct<
                     la::VectorStaticSize<TTValue, _vsize>>,
                 ds::st::sinfo::SumSecond<la::VectorStaticSize<TTValue, _vsize>,
                                          HLDSInfo>> {
 public:
  static const unsigned vsize = _vsize;
  using TValue = TTValue;
  using TVector = la::VectorStaticSize<TValue, vsize>;
  using TData = std::pair<TValue, TVector>;
  using TBase = HLD<TData, ds::st::info::SumFirst<TValue>,
                    ds::st::action::AddEachSDotProduct<TVector>,
                    ds::st::sinfo::SumSecond<TVector, HLDSInfo>>;

 public:
  HLDSDP() {}
  explicit HLDSDP(const TreeGraph& tree) { Build(tree); }
  void Build(const TreeGraph& tree) { TBase::Build(tree); }

  template <class TFunction>
  void InitSInfo(const TFunction& f) {
    for (unsigned i = 0; i < TBase::Size(); ++i) {
      auto p = TBase::Node(i);
      p->GetData().second = f(i, TBase::Deep(i));
    }
    ds::st::sinfo::SUpdateTree(TBase::stroot);
  }

  const TValue& GetValue(unsigned x) {
    auto node = TBase::Node(x);
    ds::st::action::ApplyRootToNode(node);
    return node->GetData().first;
  }

  void SetValue(unsigned x, const TValue& v) {
    auto node = TBase::Node(x);
    ds::st::action::ApplyRootToNode(node);
    node->GetData().first = v;
    ds::st::info::UpdateNodeToRoot(node);
  }
};
}  // namespace graph
