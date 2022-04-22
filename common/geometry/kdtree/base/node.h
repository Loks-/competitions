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

  void SetL(TSelf* node) { l = node; }
  void SetR(TSelf* node) { r = node; }
  void SetP(TSelf*) {}
  void ResetLinks() { l = r = nullptr; }
};

template <class TSelf>
class TNodeProxyParent<true, TSelf> : public memory::Node {
 public:
  TSelf *l = nullptr, *r = nullptr, *p = nullptr;

  void SetL(TSelf* node) {
    l = node;
    if (node) node->p = static_cast<TSelf*>(this);
  }

  void SetR(TSelf* node) {
    r = node;
    if (node) node->p = static_cast<TSelf*>(this);
  }

  void SetP(TSelf* node) { p = node; }
  void ResetLinks() { l = r = p = nullptr; }
};

template <class TTValue, class TTLData, class TTIData, class TTInfo,
          class TTAction, bool _use_parent, bool _ldata_in_inode>
class Node
    : public TNodeProxyParent<_use_parent,
                              Node<TTValue, TTLData, TTIData, TTInfo, TTAction,
                                   _use_parent, _ldata_in_inode>> {
 public:
  static const bool use_parent = _use_parent;
  static const bool ldata_in_inode = _ldata_in_inode;

  using TValue = TTValue;
  using TLData = TTLData;
  using TIData = TTIData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSelf =
      Node<TValue, TLData, TIData, TInfo, TAction, use_parent, ldata_in_inode>;
  using TProxyParent = TNodeProxyParent<use_parent, TSelf>;

  TLData ldata;
  TIData idata;
  TInfo info;
  TAction action;
  unsigned split_dim;
  TValue split_value;

  Node() : ldata() {}

  bool IsLeaf() const { return (TProxyParent::l == TProxyParent::r); }

  void ClearAction() { action.Clear(); }
  void UpdateInfo() { info.Update(this); }

  template <class TPoint>
  void UpdateLeafInfo(const TPoint& pb, const TPoint& pe) {
    idata.SetBox(pb, pe);
    info.UpdateLeaf(this, pb, pe);
  }

  template <class TActionValue>
  void AddAction(const TActionValue& value) {
    action.Add(this, value);
  }

  void ApplyAction() { action.Apply(this); }

  void ResetLinksAndUpdateInfo() {
    TProxyParent::ResetLinks();
    UpdateInfo();
  }

  void ClearReuse() {
    TProxyParent::ResetLinks();
    ClearAction();
  }
};
}  // namespace base
}  // namespace kdtree
}  // namespace geometry
