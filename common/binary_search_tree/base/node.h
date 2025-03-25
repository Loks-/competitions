#pragma once

#include "common/base.h"
#include "common/memory/node.h"

namespace bst {
namespace base {
template <bool use_key, class TKey>
class TNodeProxyKey {};

template <class TKey>
class TNodeProxyKey<false, TKey> : public memory::Node {};

template <class TKey>
class TNodeProxyKey<true, TKey> : public memory::Node {
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

  constexpr void SetL(TSelf* node) { l = node; }
  constexpr void SetR(TSelf* node) { r = node; }
  constexpr void SetP(TSelf*) {}
  constexpr void ResetLinks() { l = r = nullptr; }
};

template <bool use_key, class TKey, class TSelf>
class TNodeProxyParent<use_key, true, TKey, TSelf>
    : public TNodeProxyKey<use_key, TKey> {
 public:
  TSelf *l = nullptr, *r = nullptr, *p = nullptr;

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

template <class TTData, class TTInfo, class TTAction, bool _use_key,
          bool _use_parent = true, class TTKey = int64_t>
class Node : public TNodeProxyParent<
                 _use_key, _use_parent, TTKey,
                 Node<TTData, TTInfo, TTAction, _use_key, _use_parent, TTKey>> {
 public:
  static constexpr bool use_key = _use_key;
  static constexpr bool use_parent = _use_parent;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TKey = TTKey;
  using TSelf = Node<TData, TInfo, TAction, use_key, use_parent, TKey>;
  using TProxyParent = TNodeProxyParent<use_key, use_parent, TKey, TSelf>;

  static constexpr bool support_insert = false;
  static constexpr bool support_remove = false;

 public:
  TData data;
  TInfo subtree_data;
  TAction deferred;

 public:
  constexpr Node() : data() {}

  constexpr explicit Node(const TData& _data) : data(_data) {}

  constexpr void ClearAction() { deferred.clear(); }

  void UpdateInfo() { subtree_data.update(this); }

  void ApplyAction() { deferred.apply(this); }

  void ResetLinksAndUpdateInfo() {
    TProxyParent::ResetLinks();
    subtree_data.bti_reset();
    UpdateInfo();
  }

  constexpr void ClearCreate(unsigned raw_index) {
    subtree_data.clear_create(raw_index);
  }

  constexpr void ClearReuse() {
    TProxyParent::ResetLinks();
    ClearAction();
  }
};
}  // namespace base
}  // namespace bst
