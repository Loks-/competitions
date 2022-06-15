#pragma once

#include "common/base.h"
#include "common/data_structures/segment_tree/info/update.h"
#include "common/data_structures/segment_tree/sinfo/update.h"
#include "common/memory/node.h"

namespace ds {
namespace st {
template <bool use_parent, class TSelf>
class TNodeProxyParent {};

template <class TSelf>
class TNodeProxyParent<false, TSelf> : public memory::Node {
 public:
  TSelf *l = nullptr, *r = nullptr;

  void SetL(TSelf* node) { l = node; }
  void SetR(TSelf* node) { r = node; }
  void SetP(TSelf* node) {}
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

template <class TTData, class TTInfo, class TTAction, class TTSInfo,
          bool _use_parent = true>
class Node
    : public TNodeProxyParent<
          _use_parent, Node<TTData, TTInfo, TTAction, TTSInfo, _use_parent>> {
 public:
  static const bool use_parent = _use_parent;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSInfo = TTSInfo;
  using TCoordinate = typename TSInfo::TCoordinate;
  using TSelf = Node<TData, TInfo, TAction, TSInfo, use_parent>;
  using TBase = TNodeProxyParent<use_parent, TSelf>;

  TInfo info;
  TAction action;
  TSInfo sinfo;

  bool IsLeaf() const { return (TBase::l == nullptr); }

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
  void UpdateInfo() { info::Update(this); }
  void UpdateSInfo() { sinfo::SUpdate(this); }

  template <class TActionValue>
  void AddAction(const TActionValue& value) {
    action.Add(this, value);
  }

  void ApplyAction() { action.Apply(this); }
};
}  // namespace st
}  // namespace ds
