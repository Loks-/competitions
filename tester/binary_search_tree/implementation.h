#pragma once

#include "common/binary_search_tree/avl_tree.h"
#include "common/binary_search_tree/scapegoat_tree.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/static_tree.h"
#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/unbalanced_tree.h"
#include "common/binary_search_tree/wavl_tree.h"

#include <string_view>
#include <tuple>

namespace tester {
namespace bst {
namespace impl {

template <typename Tree>
class Base : public Tree {
 public:
  using TreeType = Tree;
  static constexpr std::string_view id() { return TreeType::id(); }
};

// AVL Tree implementations
template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPF_AVL
    : public Base<
          ::bst::AVLTree<false, Data, AggregatorsTuple, DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpf_avl"; }
};

template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPT_AVL
    : public Base<
          ::bst::AVLTree<true, Data, AggregatorsTuple, DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpt_avl"; }
};

// Scapegoat Tree implementation
template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPF_Scapegoat
    : public Base<::bst::ScapegoatTree<false, Data, AggregatorsTuple,
                                       DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpf_scapegoat"; }
};

template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPT_Scapegoat
    : public Base<::bst::ScapegoatTree<true, Data, AggregatorsTuple,
                                       DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpt_scapegoat"; }
};

// Splay Tree implementation
template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKF_HPT_Splay
    : public Base<
          ::bst::SplayTree<false, Data, AggregatorsTuple, DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpt_splay"; }
};

template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPT_Splay
    : public Base<
          ::bst::SplayTree<true, Data, AggregatorsTuple, DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpt_splay"; }
};

// Static Tree implementation
template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPF_Static
    : public Base<::bst::StaticTree<false, Data, AggregatorsTuple,
                                    DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpf_static"; }
};

template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPT_Static
    : public Base<
          ::bst::StaticTree<true, Data, AggregatorsTuple, DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpt_static"; }
};

// Treap implementation
template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKF_HPF_Treap
    : public Base<::bst::Treap<false, false, Data, AggregatorsTuple,
                               DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpf_treap"; }
};

template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKF_HPT_Treap
    : public Base<::bst::Treap<false, true, Data, AggregatorsTuple,
                               DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpt_treap"; }
};

template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPF_Treap
    : public Base<::bst::Treap<true, false, Data, AggregatorsTuple,
                               DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpf_treap"; }
};

template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPT_Treap
    : public Base<::bst::Treap<true, true, Data, AggregatorsTuple,
                               DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpt_treap"; }
};

// Unbalanced Tree implementation
template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPF_Unbalanced
    : public Base<::bst::UnbalancedTree<false, Data, AggregatorsTuple,
                                        DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpf_unbalanced"; }
};

template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPT_Unbalanced
    : public Base<::bst::UnbalancedTree<true, Data, AggregatorsTuple,
                                        DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpt_unbalanced"; }
};

// WAVL Tree implementation
template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPF_WAVL
    : public Base<
          ::bst::WAVLTree<false, Data, AggregatorsTuple, DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpf_wavl"; }
};

template <typename Data, typename Key, typename AggregatorsTuple,
          typename DeferredTuple>
class HKT_HPT_WAVL
    : public Base<
          ::bst::WAVLTree<true, Data, AggregatorsTuple, DeferredTuple, Key>> {
 public:
  static constexpr std::string_view id() { return "hpt_wavl"; }
};

}  // namespace impl
}  // namespace bst
}  // namespace tester
