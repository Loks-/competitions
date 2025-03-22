#pragma once

#include "common/base.h"
#include "common/geometry/kdtree/base/dset.h"
#include "common/geometry/kdtree/base/shift.h"
#include "common/geometry/kdtree/base/tree.h"
#include "common/geometry/kdtree/info/update_node_to_root.h"
#include "common/template.h"

#include <algorithm>

namespace geometry {
namespace kdtree {
// Base k-d tree with dynamic split when requested.
template <unsigned dim, class TTPoint, class TTLData, class TTIData,
          class TTInfo, class TTAction>
class BaseTree
    : public base::Tree<dim, TTPoint, TTLData, TTIData, TTInfo, TTAction> {
 public:
  using TPoint = TTPoint;
  using TValue = typename TPoint::TType;
  using TLData = TTLData;
  using TIData = TTIData;
  using TBase = base::Tree<dim, TPoint, TLData, TIData, TTInfo, TTAction>;
  using TNode = typename TBase::TNode;

 public:
  // Split node with coordinate from ib/ie which is closest to half by volume.
  constexpr void SplitNode(TNode* p, const TPoint& pb, const TPoint& pe,
                           const TPoint& ib, const TPoint& ie) {
    assert(p->IsLeaf());
    p->ApplyAction();
    double best_split_ratio = 0.;
    unsigned d = dim;
    TValue sv = TValue(0);
    for (unsigned i = 0; i < dim; ++i) {
      for (const auto& x : {ib[i], ie[i]}) {
        const auto ls = std::min(x - pb[i], pe[i] - x), l = pe[i] - pb[i];
        const double ds = double(ls) / double(l);
        if (best_split_ratio < ds) {
          best_split_ratio = ds;
          d = i;
          sv = x;
        }
      }
    }
    assert(d < dim);
    TBase::SplitNodeI(p, pb, pe, d, sv);
  }

  constexpr void SplitNode(TNode* p, const TPoint& ib, const TPoint& ie) {
    static_assert(TIData::support_box, "IData should contain box.");
    SplitNode(p, p->idata.b, p->idata.e, ib, ie);
  }

 protected:
  constexpr void SetI(const TPoint& pp, const TLData& ldata, MetaFalse) {
    const auto pp1 = base::Shift(pp, TValue(1));
    auto p = TBase::root;
    auto pb = TBase::sb, pe = TBase::se;
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
    for (; TBase::PossibleToSplit(pb, pe);) {
      SplitNode(p, pb, pe, pp, pp1);
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

  constexpr void SetI(const TPoint& pp, const TLData& ldata, MetaTrue) {
    const auto pp1 = base::Shift(pp, TValue(1));
    auto p = TBase::root;
    for (p->ApplyAction(); !p->IsLeaf(); p->ApplyAction())
      p = (pp[p->split_dim] < p->split_value) ? p->l : p->r;
    if (p->ldata == ldata) return;
    for (; TBase::PossibleToSplit(p->idata.b, p->idata.e);) {
      SplitNode(p, pp, pp1);
      p = (pp[p->split_dim] < p->split_value) ? p->l : p->r;
    }
    p->ldata = ldata;
    p->UpdateLeafInfo(p->idata.b, p->idata.e);
    info::UpdateNodeToRoot(p->p);
  }

 public:
  constexpr void Set(const TPoint& pp, const TLData& ldata) {
    SetI(pp, ldata, MetaBool<TIData::support_box>());
  }

 protected:
  constexpr void SetRIF(TNode* p, const TPoint& rb, const TPoint& re,
                        const TPoint& pb, const TPoint& pe, const TLData& ldata,
                        unsigned dd) {
    p->ApplyAction();
    if (dd == 0) return TBase::SetRIReplace(p, pb, pe, ldata);
    if (p->IsLeaf() && (p->ldata == ldata)) return;  // No changes
    if (p->IsLeaf()) SplitNode(p, rb, re, pb, pe);
    const unsigned d = p->split_dim;
    const TValue& v = p->split_value;
    if (pb[d] >= v) {
      SetRIF(p->r, base::DSet(d, rb, v), re, pb, pe, ldata,
             dd - ((pb[d] == v) ? 1u : 0u));
    } else if (pe[d] <= v) {
      SetRIF(p->l, rb, base::DSet(d, re, v), pb, pe, ldata,
             dd - ((pe[d] == v) ? 1u : 0u));
    } else {
      SetRIF(p->l, rb, base::DSet(d, re, v), pb, base::DSet(d, pe, v), ldata,
             dd - ((re[d] == pe[d]) ? 0u : 1u));
      SetRIF(p->r, base::DSet(d, rb, v), re, base::DSet(d, pb, v), pe, ldata,
             dd - ((rb[d] == pb[d]) ? 0u : 1u));
    }
    p->UpdateInfo();
  }

  constexpr void SetRIT(TNode* p, const TPoint& pb, const TPoint& pe,
                        const TLData& ldata, unsigned dd) {
    p->ApplyAction();
    if (dd == 0) return TBase::SetRIReplace(p, pb, pe, ldata);
    if (p->IsLeaf() && (p->ldata == ldata)) return;  // No changes
    if (p->IsLeaf()) SplitNode(p, pb, pe);
    const unsigned d = p->split_dim;
    const TValue& v = p->split_value;
    if (pb[d] >= v) {
      SetRIT(p->r, pb, pe, ldata, dd - ((pb[d] == v) ? 1u : 0u));
    } else if (pe[d] <= v) {
      SetRIT(p->l, pb, pe, ldata, dd - ((pe[d] == v) ? 1u : 0u));
    } else {
      SetRIT(p->l, pb, base::DSet(d, pe, v), ldata,
             dd - ((p->idata.e[d] == pe[d]) ? 0u : 1u));
      SetRIT(p->r, base::DSet(d, pb, v), pe, ldata,
             dd - ((p->idata.b[d] == pb[d]) ? 0u : 1u));
    }
    p->UpdateInfo();
  }

  constexpr void SetRI(TNode* p, const TPoint& pb, const TPoint& pe,
                       const TLData& ldata, unsigned dd, MetaFalse) {
    SetRIF(p, TBase::sb, TBase::se, pb, pe, ldata, dd);
  }

  constexpr void SetRI(TNode* p, const TPoint& pb, const TPoint& pe,
                       const TLData& ldata, unsigned dd, MetaTrue) {
    SetRIT(p, pb, pe, ldata, dd);
  }

 public:
  constexpr void Set(const TPoint& pb, const TPoint& pe, const TLData& ldata) {
    unsigned dd = 0;
    for (unsigned i = 0; i < dim; ++i)
      dd += ((TBase::sb[i] == pb[i]) ? 0u : 1u) +
            ((TBase::se[i] == pe[i]) ? 0u : 1u);
    SetRI(TBase::root, pb, pe, ldata, dd, MetaBool<TIData::support_box>());
  }
};
}  // namespace kdtree
}  // namespace geometry
