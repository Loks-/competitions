#pragma once

#include "common/base.h"
#include "common/geometry/kdtree/info/update_node_to_root.h"
#include "common/geometry/kdtree/node.h"
#include "common/geometry/kdtree/point_proxy.h"
#include "common/nodes_manager.h"
#include "common/template.h"

namespace geometry {
namespace kdtree {
template <unsigned _dim, class TTPoint, class TTLData, class TTIData,
          class TTInfo, class TTAction>
class ISPTree : NodesManager<Node<typename TTPoint::T, TTLData, TTIData, TTInfo,
                                  TTAction, true, false>> {
 public:
  static const unsigned dim = _dim;

  using TPoint = TTPoint;
  using TPProxy = PointProxy<TPoint>;
  using TValue = typename TPoint::T;
  using TLData = TTLData;
  using TIData = TTIData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TNode = Node<TValue, TLData, TIData, TInfo, TAction, true, false>;
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
    assert(TPProxy::StrictUnder(pb, pe));
    sb = pb;
    se = pe;
    root = New(v, pb, pe);
  }

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
      if (TPProxy::DGet(i, pe) > TPProxy::DGet(i, pb) + 1) return true;
    }
    return false;
  }

  void SplitNode(TNode* p, const TPoint& pb, const TPoint& pe) {
    assert(p->IsLeaf());
    p->ApplyAction();
    unsigned d = dim;
    TValue md = TValue(0);
    for (unsigned i = 0; i < dim; ++i) {
      auto l = TPProxy::DGet(i, pe) - TPProxy::DGet(i, pb);
      if (md < l) {
        md = l;
        d = i;
      }
    }
    assert(d < dim);
    auto v = (TPProxy::DGet(d, pe) + TPProxy::DGet(d, pb)) / 2;
    p->SetL(New(p->ldata, pb, TPProxy::DSetCopy(d, pe, v)));
    p->SetR(New(p->ldata, TPProxy::DSetCopy(d, pb, v), pe));
    p->split_dim = d;
    p->split_value = v;
  }

  void SplitNode(TNode* p) {
    static_assert(TIData::support_box, "IData should contain box.");
    SplitNode(p, p->idata.b, p->idata.e);
  }

  static TLData Get(TNode* p, const TPoint& pp) {
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction())
      p = (TPProxy::DGet(p->split_dim, pp) < p->split_value) ? p->l : p->r;
    return p->ldata;
  }

  TLData Get(const TPoint& pp) { return Get(root, pp); }

 protected:
  void SetI(const TPoint& pp, const TLData& ldata, TFakeFalse) {
    auto p = root;
    auto pb = sb, pe = se;
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction()) {
      if (TPProxy::DGet(p->split_dim, pp) < p->split_value) {
        TPProxy::DSet(p->split_dim, pe, p->split_value);
        p = p->l;
      } else {
        TPProxy::DSet(p->split_dim, pb, p->split_value);
        p = p->r;
      }
    }
    if (p->ldata == ldata) return;
    for (; PossibleToSplit(pb, pe);) {
      SplitNode(p, pb, pe);
      if (TPProxy::DGet(p->split_dim, pp) < p->split_value) {
        TPProxy::DSet(p->split_dim, pe, p->split_value);
        p = p->l;
      } else {
        TPProxy::DSet(p->split_dim, pb, p->split_value);
        p = p->r;
      }
    }
    p->ldata = ldata;
    info::UpdateNodeToRoot(p);
  }

  void SetI(const TPoint& pp, const TLData& ldata, TFakeTrue) {
    auto p = root;
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction())
      p = (TPProxy::DGet(p->split_dim, pp) < p->split_value) ? p->l : p->r;
    if (p->ldata == ldata) return;
    for (; PossibleToSplit(p->idata.b, p->idata.e);) {
      SplitNode(p);
      p = (TPProxy::DGet(p->split_dim, pp) < p->split_value) ? p->l : p->r;
    }
    p->ldata = ldata;
    info::UpdateNodeToRoot(p);
  }

 public:
  void Set(const TPoint& pp, const TLData& ldata) {
    SetI(pp, ldata, TFakeBool<TIData::support_box>());
  }

  void Add(const TPoint& pp, const TLData& ldata) { Set(pp, ldata + Get(pp)); }

 protected:
  // Get info in rectangle
  static TInfo GetInfoI(TNode* p, TPoint rb, TPoint re, const TPoint& pb,
                        const TPoint& pe) {
    p->ApplyAction();
    if (TPProxy::Under(pb, rb) && TPProxy::Under(re, pe)) return p->info;
    TInfo r;
    for (; !p->IsLeaf(); p->ApplyAction()) {
      unsigned d = p->split_dim;
      const TValue& v = p->split_value;
      if (TPProxy::DGet(d, pb) >= v) {
        p = p->r;
        TPProxy::DSet(d, rb, v);
      } else if (TPProxy::DGet(d, pe) <= v) {
        p = p->l;
        TPProxy::DSet(d, re, v);
      } else {
        r.Merge(GetInfoI(p->l, rb, TPProxy::DSetCopy(d, re, v), pb, pe));
        p = p->r;
        TPProxy::DSet(d, rb, v);
      }
    }
    r.Merge(p->info);
    return r;
  }

 public:
  TInfo GetInfo(const TPoint& pb, const TPoint& pe) {
    if (!TPProxy::StrictUnder(pb, pe)) return {};
    return GetInfoI(root, sb, se, pb, pe);
  }

  TInfo GetInfo(const TPoint& pe) { return GetInfo(sb, pe); }
};
}  // namespace kdtree
}  // namespace geometry
