#pragma once

#include "common/base.h"
#include "common/geometry/kdtree/base/dset.h"
#include "common/geometry/kdtree/base/node.h"
#include "common/geometry/kdtree/base/under.h"
#include "common/geometry/kdtree/info/update_node_to_root.h"
#include "common/memory/nodes_manager.h"
#include "common/template.h"

namespace geometry {
namespace kdtree {
namespace base {
template <unsigned _dim, class TTPoint, class TTLData, class TTIData,
          class TTInfo, class TTAction>
class Tree : protected memory::NodesManager<
                 base::Node<typename TTPoint::TType, TTLData, TTIData, TTInfo,
                            TTAction, true, false>> {
 public:
  static const unsigned dim = _dim;

  using TPoint = TTPoint;
  using TValue = typename TPoint::TType;
  using TLData = TTLData;
  using TIData = TTIData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TNode = base::Node<TValue, TLData, TIData, TInfo, TAction, true, false>;
  using TNodesManager = memory::NodesManager<TNode>;

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

 protected:
  void SplitNodeI(TNode* p, const TPoint& pb, const TPoint& pe,
                  unsigned split_dim, const TValue& split_value) {
    assert(p->IsLeaf());
    p->ApplyAction();
    p->SetL(New(p->ldata, pb, base::DSet(split_dim, pe, split_value)));
    p->SetR(New(p->ldata, base::DSet(split_dim, pb, split_value), pe));
    p->split_dim = split_dim;
    p->split_value = split_value;
  }

 public:
  void SplitNodeHLD(TNode* p, const TPoint& pb, const TPoint& pe) {
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
    SplitNodeI(p, pb, pe, d, v);
  }

  void SplitNodeHLD(TNode* p) {
    static_assert(TIData::support_box, "IData should contain box.");
    SplitNodeHLD(p, p->idata.b, p->idata.e);
  }

  static TLData Get(TNode* p, const TPoint& pp) {
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction())
      p = (pp[p->split_dim] < p->split_value) ? p->l : p->r;
    return p->ldata;
  }

  TLData Get(const TPoint& pp) { return Get(root, pp); }

 protected:
  void SetRIReplace(TNode* p, const TPoint& pb, const TPoint& pe,
                    const TLData& ldata) {
    auto pp = p->p;
    auto pnew = New(ldata, pb, pe);
    if (!pp) {
      root = pnew;
    } else if (pp->l == p) {
      pp->SetL(pnew);
    } else {
      pp->SetR(pnew);
    }
    ReleaseSubtree(p);
  }

 protected:
  // Get info in rectangle
  // Current version going to the leaf unless first node is already inside.
  // It's possible to stop earlier (check before going to both subtrees).
  // SetRI with dd is good example how to avoid unnecessary checks.
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
}  // namespace base
}  // namespace kdtree
}  // namespace geometry
