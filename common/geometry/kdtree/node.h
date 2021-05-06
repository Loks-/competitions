#pragma once

#include "common/base.h"
#include "common/node.h"

namespace geometry {
namespace kdtree {
template <bool use_parent, class TSelf>
class TNodeProxyParent {};

template <class TSelf>
class TNodeProxyParent<false, TSelf>
    : public BaseNode {
 public:
  TSelf *l = nullptr, *r = nullptr;

  void SetL(TSelf* node) { l = node; }
  void SetR(TSelf* node) { r = node; }
  void SetP(TSelf*) {}
  void ResetLinks() { l = r = nullptr; }
};

template <class TSelf>
class TNodeProxyParent<true, TSelf>
    : public BaseNode {
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

template <class TTValue, class TTData, class TTInfo, class TTAction,
          bool _use_parent = true>
class Node : public TNodeProxyParent<_use_parent,
                 Node<TTValue, TTData, TTInfo, TTAction, _use_parent>> {
 public:
  static const bool use_parent = _use_parent;

  using TValue = TTValue;
  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSelf = Node<TData, TInfo, TAction, use_parent>;
  using TProxyParent = TNodeProxyParent<use_parent, TSelf>;

  TData data;
  TInfo info;
  TAction action;

  Node() : data() {}
  explicit Node(const TData& _data) : data(_data) {}

  void ClearAction() { action.Clear(); }
  void UpdateInfo() { info.Update(this); }
  template <class TActionValue>
  void AddAction(const TActionValue& value) {
    action.Add(this, value);
  }

  void ApplyAction() { action.Apply(this); }

  void ResetLinksAndUpdateInfo() {
    TProxyParent::ResetLinks();
    info.BTIReset();
    UpdateInfo();
  }

  void ClearReuse() {
    TProxyParent::ResetLinks();
    ClearAction();
  }
};
}  // namespace kdtree
}  // namespace geometry
