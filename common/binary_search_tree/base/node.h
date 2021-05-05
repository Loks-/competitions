#pragma once

#include "common/base.h"
#include "common/node.h"

namespace bst {
namespace base {
template <bool use_key, class TKey>
class TNodeProxyKey {};

template <class TKey>
class TNodeProxyKey<false, TKey> : public BaseNode {};

template <class TKey>
class TNodeProxyKey<true, TKey> : public BaseNode {
 public:
  TKey key;
};

template <bool use_key, bool use_parent, class TKey, class TSelf>
class TNodeProxyParent {};

template <bool use_key, class TKey, class TSelf>
class TNodeProxyParent<use_key, false, TKey, TSelf>
    : public TNodeProxyKey<use_key, TKey> {
 public:
  TSelf *l = nullptr, *r = nullptr;

  void SetL(TSelf* node) { l = node; }
  void SetR(TSelf* node) { r = node; }
  void SetP(TSelf*) {}
  void ResetLinks() { l = r = nullptr; }
};

template <bool use_key, class TKey, class TSelf>
class TNodeProxyParent<use_key, true, TKey, TSelf>
    : public TNodeProxyKey<use_key, TKey> {
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

template <class TTData, class TTInfo, class TTAction, bool _use_key,
          bool _use_parent = true, class TTKey = int64_t>
class Node : public TNodeProxyParent<
                 _use_key, _use_parent, TTKey,
                 Node<TTData, TTInfo, TTAction, _use_key, _use_parent, TTKey>> {
 public:
  static const bool use_key = _use_key;
  static const bool use_parent = _use_parent;
  static const bool support_insert = false;
  static const bool support_remove = false;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TKey = TTKey;
  using TSelf = Node<TData, TInfo, TAction, use_key, use_parent, TKey>;
  using TProxyParent = TNodeProxyParent<use_key, use_parent, TKey, TSelf>;

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
}  // namespace base
}  // namespace bst
