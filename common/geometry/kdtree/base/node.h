#pragma once

#include "common/base.h"
#include "common/memory/node.h"

namespace geometry {
namespace kdtree {
namespace base {
template <bool use_parent, class TSelf>
class TNodeProxyParent {};

template <class TSelf>
class TNodeProxyParent<false, TSelf> : public memory::Node {
 public:
  TSelf *l = nullptr, *r = nullptr;

 public:
  constexpr void SetL(TSelf* node) { l = node; }
  constexpr void SetR(TSelf* node) { r = node; }
  constexpr void SetP(TSelf*) {}
  constexpr void ResetLinks() { l = r = nullptr; }
};

template <class TSelf>
class TNodeProxyParent<true, TSelf> : public memory::Node {
 public:
  TSelf *l = nullptr, *r = nullptr, *p = nullptr;

 public:
  constexpr void SetL(TSelf* node) {
    l = node;
    if (node) node->p = static_cast<TSelf*>(this);
  }

  constexpr void SetR(TSelf* node) {
    r = node;
    if (node) node->p = static_cast<TSelf*>(this);
  }

  constexpr void SetP(TSelf* node) { p = node; }

  constexpr void ResetLinks() { l = r = p = nullptr; }
};

template <class TTValue, class TTLData, class TTIData, class TTInfo,
          class TTAction, bool _use_parent, bool _ldata_in_inode>
class Node
    : public TNodeProxyParent<_use_parent,
                              Node<TTValue, TTLData, TTIData, TTInfo, TTAction,
                                   _use_parent, _ldata_in_inode>> {
 public:
  static constexpr bool use_parent = _use_parent;
  static constexpr bool ldata_in_inode = _ldata_in_inode;

  using TValue = TTValue;
  using TLData = TTLData;
  using TIData = TTIData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSelf =
      Node<TValue, TLData, TIData, TInfo, TAction, use_parent, ldata_in_inode>;
  using TProxyParent = TNodeProxyParent<use_parent, TSelf>;

 public:
  TLData ldata;
  TIData idata;
  TInfo info;
  TAction action;
  unsigned split_dim;
  TValue split_value;

 public:
  constexpr Node() : ldata() {}

  constexpr bool IsLeaf() const { return (TProxyParent::l == TProxyParent::r); }

  constexpr void ClearAction() { action.Clear(); }
  constexpr void UpdateInfo() { info.Update(this); }

  template <class TPoint>
  constexpr void UpdateLeafInfo(const TPoint& pb, const TPoint& pe) {
    idata.SetBox(pb, pe);
    info.UpdateLeaf(this, pb, pe);
  }

  template <class TActionValue>
  constexpr void AddAction(const TActionValue& value) {
    action.Add(this, value);
  }

  constexpr void ApplyAction() { action.Apply(this); }

  constexpr void ResetLinksAndUpdateInfo() {
    TProxyParent::ResetLinks();
    UpdateInfo();
  }

  constexpr void ClearReuse() {
    TProxyParent::ResetLinks();
    ClearAction();
  }
};
}  // namespace base
}  // namespace kdtree
}  // namespace geometry
