#pragma once

#include "common/base.h"
#include "common/node.h"

#include <random>

namespace bst {
template <bool use_height, class THeight>
class TNodeProxyHeight {};

template <class THeight>
class TNodeProxyHeight<false, THeight> : public BaseNode {};

template <class THeight>
class TNodeProxyHeight<true, THeight> : public BaseNode {
 public:
  THeight height;

  TNodeProxyHeight() {
    thread_local std::minstd_rand random_engine;
    height = random_engine();
  }
};

template <bool use_key, bool use_height, class TKey, class Height>
class TNodeProxyKey {};

template <bool use_height, class TKey, class THeight>
class TNodeProxyKey<false, use_height, TKey, THeight>
    : public TNodeProxyHeight<use_height, THeight> {};

template <bool use_height, class TKey, class THeight>
class TNodeProxyKey<true, use_height, TKey, THeight>
    : public TNodeProxyHeight<use_height, THeight> {
 public:
  TKey key;
};

template <bool use_key, bool use_parent, bool use_height, class TKey,
          class THeight, class TSelf>
class TNodeProxyParent {};

template <bool use_key, bool use_height, class TKey, class THeight, class TSelf>
class TNodeProxyParent<use_key, false, use_height, TKey, THeight, TSelf>
    : public TNodeProxyKey<use_key, use_height, TKey, THeight> {
 public:
  TSelf *l = nullptr, *r = nullptr;

  void SetL(TSelf* node) { l = node; }
  void SetR(TSelf* node) { r = node; }
  void SetP(TSelf*) {}
  void ResetLinks() { l = r = nullptr; }
};

template <bool use_key, bool use_height, class TKey, class THeight, class TSelf>
class TNodeProxyParent<use_key, true, use_height, TKey, THeight, TSelf>
    : public TNodeProxyKey<use_key, use_height, TKey, THeight> {
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
          bool _use_parent = true, bool _use_height = false,
          class TTKey = int64_t, class TTHeight = unsigned>
class Node
    : public TNodeProxyParent<_use_key, _use_parent, _use_height, TTKey,
                              TTHeight,
                              Node<TTData, TTInfo, TTAction, _use_key,
                                   _use_parent, _use_height, TTKey, TTHeight>> {
 public:
  static const bool use_key = _use_key;
  static const bool use_parent = _use_parent;
  static const bool use_height = _use_height;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TKey = TTKey;
  using THeight = TTHeight;
  using TSelf = Node<TData, TInfo, TAction, use_key, use_parent, use_height,
                     TKey, THeight>;
  using TProxyParent =
      TNodeProxyParent<use_key, use_parent, use_height, TKey, THeight, TSelf>;

  static const bool support_insert = TInfo::support_insert;
  static const bool support_remove = TInfo::support_remove;

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
    UpdateInfo();
  }

  void ClearReuse() {
    TProxyParent::ResetLinks();
    ClearAction();
  }
};
}  // namespace bst
