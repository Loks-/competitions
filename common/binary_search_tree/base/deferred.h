#pragma once

#include "common/base.h"
#include "common/templates/tuple.h"

#include <tuple>
#include <type_traits>

namespace bst {
namespace base {

/**
 * @brief A class that manages multiple deferred computations in a BST.
 *
 * This class maintains a tuple of deferred computation managers and provides
 * a unified interface to access and apply them. It combines flags from all
 * deferred computations and provides methods to apply them in sequence.
 *
 * WARNING: While this class supports multiple types of deferred computations,
 * it does not guarantee any specific order of application between different
 * computation types. Only deferred computations within the same class have
 * a guaranteed order. When using multiple deferred computation types, they
 * must be order-independent (e.g., one affecting node data and another
 * affecting node keys).
 *
 * @tparam TDeferredTuple A tuple of deferred computation types.
 */
template <class TDeferredTuple>
class Deferred {
 public:
  using Self = Deferred;
  using Tuple = TDeferredTuple;

  /**
   * @brief Helper struct to compute combined flags from deferred computations.
   * Primary template is not defined - only the specialization for std::tuple is
   * used.
   */
  template <typename T>
  struct ComputeFlags;

  /**
   * @brief Specialization that unpacks the tuple's types into Types...
   * Computes combined flags using fold expressions over the tuple elements.
   */
  template <typename... Types>
  struct ComputeFlags<std::tuple<Types...>> {
    static constexpr bool empty = sizeof...(Types) == 0;
    static constexpr bool modify_data =
        empty ? false : (Types::modify_data || ...);
    static constexpr bool modify_keys =
        empty ? false : (Types::modify_keys || ...);
    static constexpr bool modify_tree_structure =
        empty ? false : (Types::modify_tree_structure || ...);
    static constexpr bool modify_nodes_order =
        empty ? false : (Types::modify_nodes_order || ...);
  };

  /**
   * @brief Combined flags from all deferred computations in the tuple.
   */
  using Flags = ComputeFlags<Tuple>;

  /**
   * @brief Flag indicating if there are any deferred computations.
   */
  static constexpr bool empty = Flags::empty;

  /**
   * @brief Flag indicating if any deferred operations modify node data.
   */
  static constexpr bool modify_data = Flags::modify_data;

  /**
   * @brief Flag indicating if any deferred operations modify node keys.
   */
  static constexpr bool modify_keys = Flags::modify_keys;

  /**
   * @brief Flag indicating if any deferred operations modify tree structure.
   */
  static constexpr bool modify_tree_structure = Flags::modify_tree_structure;

  /**
   * @brief Flag indicating if any deferred operations modify node order.
   */
  static constexpr bool modify_nodes_order = Flags::modify_nodes_order;

  /**
   * @brief Checks at compile time if a specific deferred computation type is
   * present.
   *
   * @tparam T The type to check for in the tuple.
   * @return true if type T is present in deferred computations, false
   * otherwise.
   */
  template <typename T>
  static constexpr bool has() {
    return templates::HasType<T, Tuple>::value;
  }

  /**
   * @brief Gets a reference to a specific deferred computation.
   *
   * @tparam T The type of deferred computation to access.
   * @return Reference to the requested deferred computation.
   */
  template <class T>
  constexpr T& get() {
    return std::get<T>(deferred);
  }

  /**
   * @brief Gets a const reference to a specific deferred computation.
   *
   * @tparam T The type of deferred computation to access.
   * @return Const reference to the requested deferred computation.
   */
  template <class T>
  constexpr const T& get() const {
    return std::get<T>(deferred);
  }

  /**
   * @brief Resets all deferred computations to empty state.
   */
  constexpr void clear() {
    std::apply([](auto&... x) { (x.clear(), ...); }, deferred);
  }

  /**
   * @brief Checks if any deferred computations need to be applied.
   *
   * @return true if any computation needs to be applied, false otherwise.
   */
  constexpr bool apply_required() const {
    return std::apply(
        [](const auto&... x) { return (x.apply_required() || ...); }, deferred);
  }

  /**
   * @brief Applies all pending deferred computations.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to apply computations to.
   */
  template <class Node>
  constexpr void apply(Node* node) {
    std::apply([node](auto&... x) { (x.apply(node), ...); }, deferred);
  }

  /**
   * @brief Notifies that a subtree has been reversed.
   *
   * This function is called when a subtree is reversed, allowing all deferred
   * computations to update their state accordingly. It calls reverse_subtree
   * on each deferred computation in the tuple.
   *
   * @tparam Node The BST node type.
   * @param node The root of the reversed subtree.
   */
  template <class Node>
  constexpr void reverse_subtree(Node* node) {
    std::apply([node](auto&... x) { (x.reverse_subtree(node), ...); },
               deferred);
  }

 protected:
  /**
   * @brief Tuple containing all deferred computations.
   */
  Tuple deferred{};
};

}  // namespace base
}  // namespace bst
