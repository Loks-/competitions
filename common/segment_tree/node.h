#pragma once

#include "../base.h"

template <bool use_parent, class TSelf>
class TSTNodeProxyParent {};

template <class TSelf>
class TSTNodeProxyParent<false, TSelf> {
 public:
  TSelf *l = 0, *r = 0;

  void SetL(TSelf* node) { l = node; }
  void SetR(TSelf* node) { r = node; }
  void SetP(TSelf* node) {}
  void ResetLinks() { l = r = 0; }
};

template <class TSelf>
class TSTNodeProxyParent<true, TSelf> {
 public:
  TSelf *l = 0, *r = 0, *p = 0;

  void SetL(TSelf* node) {
    l = node;
    if (node) node->p = static_cast<TSelf*>(this);
  }

  void SetR(TSelf* node) {
    r = node;
    if (node) node->p = static_cast<TSelf*>(this);
  }

  void SetP(TSelf* node) { p = node; }
  void ResetLinks() { l = r = p = 0; }
};

template <class TTData, class TTInfo, class TTAction, bool _use_parent = true>
class STNode
    : public TSTNodeProxyParent<_use_parent,
                                STNode<TTData, TTInfo, TTAction, _use_parent>> {
 public:
  static const bool use_parent = _use_parent;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSelf = STNode<TData, TInfo, TAction, use_parent>;
  using TBase = TSTNodeProxyParent<use_parent, TSelf>;

  TInfo info;
  TAction action;

  bool IsLeaf() const { return (TBase::l == 0); }

  TData& GetData() {
    assert(IsLeaf());
    return *reinterpret_cast<TData*>(TBase::r);
  }

  const TData& GetData() const {
    assert(IsLeaf());
    return *reinterpret_cast<const TData*>(TBase::r);
  }

  void SetPData(TData* p) {
    assert(IsLeaf());
    TBase::r = reinterpret_cast<TSelf*>(p);
  }

  void ClearAction() { action.Clear(); }
  void UpdateInfo() { info.Update(this); }

  template <class TActionValue>
  void AddAction(const TActionValue& value) {
    action.Add(this, value);
  }

  void ApplyAction() { action.Apply(this); }
};
