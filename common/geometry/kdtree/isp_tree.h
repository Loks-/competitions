#pragma once

#include "common/base.h"
#include "common/geometry/kdtree/get_set.h"
#include "common/geometry/kdtree/idata/none.h"
#include "common/geometry/kdtree/info/none.h"
#include "common/geometry/kdtree/info/update_node_to_root.h"
#include "common/geometry/kdtree/node.h"
#include "common/nodes_manager.h"
#include "common/template.h"

namespace geometry {
namespace kdtree {
template <unsigned _dim, class TTPoint, class TTLData = int64_t,
          class TTIData = idata::None, class TTInfo = info::None,
          class TTAction = TEmpty>
class ISPTree
    : NodesManager<
          Node<typename TTPoint::T, TTLData, TTIData, TTInfo, TTAction, true>> {
 public:
  static const unsigned dim = _dim;

  using TPoint = TTPoint;
  using TValue = typename TPoint::T;
  using TLData = TTLData;
  using TIData = TTIData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TNode = Node<TValue, TLData, TIData, TInfo, TAction, true>;
  using TNodesManager = NodesManager<TNode>;

 public:
  TPoint sb, se;
  TNode* root;

 public:
  TNode* New() { return TNodesManager::New(); }

  TNode* New(const TLData& ldata, const TPoint& pb, const TPoint& pe) {
    auto p = New();
    p->ldata = ldata;
    p->idata.SetBox(pb, pe);
    p->UpdateInfo();
    return p;
  }

  bool PossibleToSplit(const TPoint& pb, const TPoint& pe) {
    for (unsigned i = 0; i < dim; ++i) {
      if (GetD(i, pe) > GetD(i, pb) + 1) return true;
    }
    return false;
  }

  void SplitNode(TNode* p, const TPoint& pb, const TPoint& pe) {
    assert(p->IsLeaf());
    p->ApplyAction();
    unsigned d = dim;
    TValue md = TValue(0);
    for (unsigned i = 0; i < dim; ++i) {
      auto l = GetD(i, pe) - GetD(i, pb);
      if (md < l) {
        md = l;
        d = i;
      }
    }
    assert(d < dim);
    auto v = (GetD(d, pe) + GetD(d, pb)) / 2;
    p->SetL(New(p->ldata, pb, DSetCopy(d, pe, v)));
    p->SetR(New(p->ldata, DSetCopy(d, pb, v), pe));
    p->split_dim = d;
    p->split_value = v;
  }

  void SplitNode(TNode* p) {
    static_assert(TIData::support_box, "IData should contain box.");
    SplitNode(p, p->idata.b, p->idata.e);
  }

  static TLData Get(const TNode* p, const TPoint& pp) {
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction())
      p = (DGet(p->split_dim, pp) < p->split_value) ? p->l : p->r;
    return p->idata;
  }

  TLData Get(const TPoint& pp) const { return Get(root, pp); }

 protected:
  void SetI(const TPoint& pp, const TLData& ldata, TFakeFalse) {
    auto p = root;
    auto pb = sb, pe = se;
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction()) {
      if (DGet(p->split_dim, pp) < p->split_value) {
        DSet(p->split_dim, pe, p->split_value);
        p = p->l;
      } else {
        DSet(p->split_dim, pb, p->split_value);
        p = p->r;
      }
    }
    if (p->ldata == ldata) return;
    for (; PossibleToSplit(pb, pe);) {
      SplitNode(p, pb, pe);
      if (DGet(p->split_dim, pp) < p->split_value) {
        DSet(p->split_dim, pe, p->split_value);
        p = p->l;
      } else {
        DSet(p->split_dim, pb, p->split_value);
        p = p->r;
      }
    }
    p->ldata = ldata;
    info::UpdateNodeToRoot(p);
  }

  void SetI(const TPoint& pp, const TLData& ldata, TFakeTrue) {
    auto p = root;
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction())
      p = (DGet(p->split_dim, pp) < p->split_value) ? p->l : p->r;
    if (p->ldata == ldata) return;
    for (; PossibleToSplit(p->idata.b, p->idata.e);) {
      SplitNode(p);
      p = (DGet(p->split_dim, pp) < p->split_value) ? p->l : p->r;
    }
    p->ldata = ldata;
    info::UpdateNodeToRoot(p);
  }

 public:
  void Set(const TPoint& pp, const TLData& ldata) {
    SetI(pp, ldata, TFakeBool<TIData::support_box>());
  }

  void Add(const TPoint& pp, const TLData& ldata) { Set(pp, ldata + Get(pp)); }
};
}  // namespace kdtree
}  // namespace geometry
