#pragma once

#include "common/base.h"
#include "common/geometry/kdtree/base/dset.h"
#include "common/geometry/kdtree/base/node.h"
#include "common/geometry/kdtree/base/under.h"
#include "common/geometry/kdtree/info/update_node_to_root.h"
#include "common/nodes_manager.h"
#include "common/template.h"

namespace geometry {
namespace kdtree {
template <unsigned _dim, class TTPoint, class TTLData, class TTIData,
          class TTInfo, class TTAction>
class ISPTree : NodesManager<base::Node<typename TTPoint::T, TTLData, TTIData,
                                        TTInfo, TTAction, true, false>> {
 public:
  static const unsigned dim = _dim;

  using TPoint = TTPoint;
  using TValue = typename TPoint::T;
  using TLData = TTLData;
  using TIData = TTIData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TNode = base::Node<TValue, TLData, TIData, TInfo, TAction, true, false>;
  using TNodesManager = NodesManager<TNode>;

 public:
  TPoint sb, se;
  TNode* root = nullptr;

 protected:
  void ReleaseSubtree(TNode* p) {
    if (p) {
      ReleaseSubtree(p->l);
      ReleaseSubtree(p->r);
      TNodesManager::Release(p);
    }
  }

 public:
  void Clear() {
    ReleaseSubtree(root);
    root = nullptr;
  }

  void Init(const TPoint& pb, const TPoint& pe, const TValue& v = TValue()) {
    Clear();
    assert(base::StrictUnder(pb, pe));
    sb = pb;
    se = pe;
    root = New(v, pb, pe);
  }

  TNode* New() { return TNodesManager::New(); }

  TNode* New(const TLData& ldata, const TPoint& pb, const TPoint& pe) {
    auto p = New();
    p->ldata = ldata;
    p->UpdateLeafInfo(pb, pe);
    return p;
  }

  bool PossibleToSplit(const TPoint& pb, const TPoint& pe) {
    for (unsigned i = 0; i < dim; ++i) {
      if (pe[i] > pb[i] + 1) return true;
    }
    return false;
  }

  void SplitNode(TNode* p, const TPoint& pb, const TPoint& pe) {
    assert(p->IsLeaf());
    p->ApplyAction();
    unsigned d = dim;
    TValue md = TValue(0);
    for (unsigned i = 0; i < dim; ++i) {
      auto l = pe[i] - pb[i];
      if (md < l) {
        md = l;
        d = i;
      }
    }
    assert(d < dim);
    auto v = (pb[d] + pe[d]) / 2;
    p->SetL(New(p->ldata, pb, base::DSet(d, pe, v)));
    p->SetR(New(p->ldata, base::DSet(d, pb, v), pe));
    p->split_dim = d;
    p->split_value = v;
  }

  void SplitNode(TNode* p) {
    static_assert(TIData::support_box, "IData should contain box.");
    SplitNode(p, p->idata.b, p->idata.e);
  }

  static TLData Get(TNode* p, const TPoint& pp) {
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction())
      p = (pp[p->split_dim] < p->split_value) ? p->l : p->r;
    return p->ldata;
  }

  TLData Get(const TPoint& pp) { return Get(root, pp); }

 protected:
  void SetI(const TPoint& pp, const TLData& ldata, TFakeFalse) {
    auto p = root;
    auto pb = sb, pe = se;
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction()) {
      if (pp[p->split_dim] < p->split_value) {
        pe[p->split_dim] = p->split_value;
        p = p->l;
      } else {
        pb[p->split_dim] = p->split_value;
        p = p->r;
      }
    }
    if (p->ldata == ldata) return;
    for (; PossibleToSplit(pb, pe);) {
      SplitNode(p, pb, pe);
      if (pp[p->split_dim] < p->split_value) {
        pe[p->split_dim] = p->split_value;
        p = p->l;
      } else {
        pb[p->split_dim] = p->split_value;
        p = p->r;
      }
    }
    p->ldata = ldata;
    p->UpdateLeafInfo(pb, pe);
    info::UpdateNodeToRoot(p->p);
  }

  void SetI(const TPoint& pp, const TLData& ldata, TFakeTrue) {
    auto p = root;
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction())
      p = (pp[p->split_dim] < p->split_value) ? p->l : p->r;
    if (p->ldata == ldata) return;
    for (; PossibleToSplit(p->idata.b, p->idata.e);) {
      SplitNode(p);
      p = (pp[p->split_dim] < p->split_value) ? p->l : p->r;
    }
    p->ldata = ldata;
    p->UpdateLeafInfo(p->idata.b, p->idata.e);
    info::UpdateNodeToRoot(p->p);
  }

 public:
  void Set(const TPoint& pp, const TLData& ldata) {
    SetI(pp, ldata, TFakeBool<TIData::support_box>());
  }

  void Add(const TPoint& pp, const TLData& ldata) { Set(pp, ldata + Get(pp)); }

 protected:
  // Get info in rectangle
  // Current version going to the leaf unless first node is already inside.
  // It's possible to stop earlier (check before going to both subtrees).
  static TInfo GetInfoI(TNode* p, TPoint rb, TPoint re, const TPoint& pb,
                        const TPoint& pe) {
    p->ApplyAction();
    if (base::Under(pb, rb) && base::Under(re, pe)) return p->info;
    TInfo r;
    for (; !p->IsLeaf(); p->ApplyAction()) {
      unsigned d = p->split_dim;
      const TValue& v = p->split_value;
      if (pb[d] >= v) {
        p = p->r;
        rb[d] = v;
      } else if (pe[d] <= v) {
        p = p->l;
        re[d] = v;
      } else {
        r.Merge(GetInfoI(p->l, rb, base::DSet(d, re, v), pb, pe));
        p = p->r;
        rb[d] = v;
      }
    }
    r.Merge(p->info);
    return r;
  }

 public:
  TInfo GetInfo(const TPoint& pb, const TPoint& pe) {
    if (!base::StrictUnder(pb, pe)) return {};
    return GetInfoI(root, sb, se, pb, pe);
  }

  TInfo GetInfo(const TPoint& pe) { return GetInfo(sb, pe); }
};
}  // namespace kdtree
}  // namespace geometry
