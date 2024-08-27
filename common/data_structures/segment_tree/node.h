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

 public:
  constexpr void SetL(TSelf* node) { l = node; }
  constexpr void SetR(TSelf* node) { r = node; }
  constexpr void SetP(TSelf* node) {}
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

 public:
  TInfo info;
  TAction action;
  TSInfo sinfo;

 public:
  constexpr bool IsLeaf() const { return (TBase::l == nullptr); }

  constexpr TData& GetData() {
    assert(IsLeaf());
    return *reinterpret_cast<TData*>(TBase::r);
  }

  constexpr const TData& GetData() const {
    assert(IsLeaf());
    return *reinterpret_cast<const TData*>(TBase::r);
  }

  constexpr void SetPData(TData* p) {
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
