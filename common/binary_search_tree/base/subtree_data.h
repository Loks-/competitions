#pragma once

#include "common/base.h"
#include "common/templates/tuple.h"

#include <tuple>
#include <type_traits>

namespace bst {
namespace base {

/**
 * @brief Primary template for managing multiple subtree data aggregators.
 *
 * This class combines multiple aggregators into a single component using
 * tuple storage. It automatically computes capability flags across all
 * aggregators and provides unified access to aggregator operations.
 *
 * @tparam Tuple A std::tuple containing aggregator types.
 */
template <typename Tuple>
class SubtreeData {
 public:
  using Self = SubtreeData<Tuple>;
  using TupleType = Tuple;

  // Helper to get tuple size at compile time
  static constexpr size_t aggregator_count = std::tuple_size_v<Tuple>;

  /**
   * @brief Helper type trait to compute combined flags from tuple elements.
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
    static constexpr bool empty = (aggregator_count == 0);

    // OR flags - true if any aggregator needs it
    static constexpr bool use_data = empty ? false : (Types::use_data || ...);
    static constexpr bool use_keys = empty ? false : (Types::use_keys || ...);
    static constexpr bool has_size = empty ? false : (Types::has_size || ...);

    // AND flags - true only if all aggregators support it
    static constexpr bool support_segment =
        empty ? true : (Types::support_segment && ...);
    static constexpr bool order_independent =
        empty ? true : (Types::order_independent && ...);
    static constexpr bool support_insert_node =
        empty ? true : (Types::support_insert_node && ...);
    static constexpr bool support_insert_subtree =
        empty ? true : (Types::support_insert_subtree && ...);
    static constexpr bool support_remove_node =
        empty ? true : (Types::support_remove_node && ...);
  };

  /**
   * @brief Combined capability flags across all aggregators.
   */
  using Flags = ComputeFlags<Tuple>;
  static constexpr bool empty = Flags::empty;
  static constexpr bool use_data = Flags::use_data;
  static constexpr bool use_keys = Flags::use_keys;
  static constexpr bool has_size = Flags::has_size;
  static constexpr bool support_segment = Flags::support_segment;
  static constexpr bool order_independent = Flags::order_independent;
  static constexpr bool support_insert_node = Flags::support_insert_node;
  static constexpr bool support_insert_subtree = Flags::support_insert_subtree;
  static constexpr bool support_remove_node = Flags::support_remove_node;

  /**
   * @brief Checks at compile time if a specific aggregator type is present.
   *
   * @tparam T The type to check for in the tuple.
   * @return true if type T is present in aggregators, false otherwise.
   */
  template <typename T>
  static constexpr bool has() {
    return templates::HasType<T, Tuple>::value;
  }

  /**
   * @brief Gets a specific aggregator from the tuple.
   *
   * @tparam T The type of aggregator to retrieve.
   * @return Reference to the requested aggregator.
   */
  template <typename T>
  constexpr T& get() {
    return std::get<T>(aggregators_);
  }

  /**
   * @brief Gets a specific aggregator from the tuple (const version).
   *
   * @tparam T The type of aggregator to retrieve.
   * @return Const reference to the requested aggregator.
   */
  template <typename T>
  constexpr const T& get() const {
    return std::get<T>(aggregators_);
  }

  /**
   * @brief Initializes a new node by calling clear_create on all aggregators.
   *
   * @param raw_index The unique index of the node to initialize.
   */
  constexpr void clear_create(unsigned raw_index) {
    std::apply(
        [raw_index](auto&... aggregators) {
          (aggregators.clear_create(raw_index), ...);
        },
        aggregators_);
  }

  /**
   * @brief Resets the balance tree data for all aggregators.
   */
  constexpr void bti_reset() {
    std::apply([](auto&... aggregators) { (aggregators.bti_reset(), ...); },
               aggregators_);
  }

  /**
   * @brief Copies balance tree data from another node for all aggregators.
   *
   * @tparam Node The type of the source node.
   * @param node Pointer to the source node to copy from.
   */
  template <typename Node>
  constexpr void bti_copy(const Node* node) {
    assert(node);
    std::apply(
        [node](auto&... aggregators) { (aggregators.bti_copy(node), ...); },
        aggregators_);
  }

  /**
   * @brief Swaps balance tree data with another node for all aggregators.
   *
   * @tparam Node The type of the target node.
   * @param node Pointer to the node to swap with.
   */
  template <typename Node>
  constexpr void bti_swap(Node* node) {
    assert(node);
    std::apply(
        [node](auto&... aggregators) { (aggregators.bti_swap(node), ...); },
        aggregators_);
  }

  /**
   * @brief Updates the subtree data for all aggregators.
   *
   * For segment-supporting case:
   * 1. Sets data from the current node
   * 2. Adds data from left subtree if it exists
   * 3. Adds data from right subtree if it exists
   *
   * For non-segment case:
   * Calls update_one for each aggregator.
   *
   * @tparam Node The type of the node to update.
   * @param node Pointer to the node whose subtree data needs to be updated.
   */
  template <typename Node>
  constexpr void update(const Node* node) {
    assert(node);
    if constexpr (support_segment) {
      set_node(node);
      if (node->left) add_left_subtree(node->left);
      if (node->right) add_right_subtree(node->right);
    } else {
      std::apply(
          [node](auto&... aggregators) {
            (update_one(aggregators, node), ...);
          },
          aggregators_);
    }
  }

  /**
   * @brief Sets the subtree data from a single node for all aggregators.
   *
   * @tparam Node The type of the source node.
   * @param node Pointer to the node to set data from.
   */
  template <typename Node>
  constexpr void set_node(const Node* node) {
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (set_node_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Sets the subtree data from a subtree for all aggregators.
   *
   * @tparam Node The type of the source node.
   * @param node Pointer to the root of the subtree to set data from.
   */
  template <typename Node>
  constexpr void set_subtree(const Node* node) {
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (set_subtree_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Adds the subtree data from a single node for all aggregators.
   * Only available when all aggregators are order independent.
   *
   * @tparam Node The type of the source node.
   * @param node Pointer to the node to add data from.
   */
  template <typename Node>
  constexpr void add_node(const Node* node) {
    static_assert(order_independent,
                  "add_node can only be called when all aggregators are "
                  "order independent");
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (add_node_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Adds the subtree data from a subtree for all aggregators.
   * Only available when all aggregators are order independent.
   *
   * @tparam Node The type of the source node.
   * @param node Pointer to the root of the subtree to add data from.
   */
  template <typename Node>
  constexpr void add_subtree(const Node* node) {
    static_assert(order_independent,
                  "add_subtree can only be called when all aggregators are "
                  "order independent");
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (add_subtree_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Adds the subtree data from a single node for all aggregators.
   * Used for order-dependent operations.
   *
   * @tparam Node The type of the source node.
   * @param node Pointer to the node to add data from.
   */
  template <typename Node>
  constexpr void add_left_node(const Node* node) {
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (add_left_node_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Adds the subtree data from a single node for all aggregators.
   * Used for order-dependent operations.
   *
   * @tparam Node The type of the source node.
   * @param node Pointer to the node to add data from.
   */
  template <typename Node>
  constexpr void add_right_node(const Node* node) {
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (add_right_node_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Adds the subtree data from a subtree for all aggregators.
   * Used for order-dependent operations.
   *
   * @tparam Node The type of the source node.
   * @param node Pointer to the root of the subtree to add data from.
   */
  template <typename Node>
  constexpr void add_left_subtree(const Node* node) {
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (add_left_subtree_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Adds the subtree data from a subtree for all aggregators.
   * Used for order-dependent operations.
   *
   * @tparam Node The type of the source node.
   * @param node Pointer to the root of the subtree to add data from.
   */
  template <typename Node>
  constexpr void add_right_subtree(const Node* node) {
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (add_right_subtree_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Handles node insertion in the subtree data for all aggregators.
   * Only available when all aggregators support node insertion.
   *
   * @tparam Node The type of the node to insert.
   * @param node Pointer to the node being inserted.
   */
  template <typename Node>
  constexpr void insert_node(const Node* node) {
    static_assert(support_insert_node,
                  "insert_node can only be called when all aggregators support "
                  "node insertion");
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (insert_node_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Handles subtree insertion in the subtree data for all aggregators.
   * Only available when all aggregators support subtree insertion.
   *
   * @tparam Node The type of the node to insert.
   * @param node Pointer to the root of the subtree being inserted.
   */
  template <typename Node>
  constexpr void insert_subtree(const Node* node) {
    static_assert(
        support_insert_subtree,
        "insert_subtree can only be called when all aggregators support "
        "subtree insertion");
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (insert_subtree_one(aggregators, node), ...);
        },
        aggregators_);
  }

  /**
   * @brief Handles node removal from the subtree data for all aggregators.
   * Only available when all aggregators support node removal.
   *
   * @tparam Node The type of the node to remove.
   * @param node Pointer to the node being removed.
   */
  template <typename Node>
  constexpr void remove_node(const Node* node) {
    static_assert(support_remove_node,
                  "remove_node can only be called when all aggregators support "
                  "node removal");
    assert(node);
    std::apply(
        [node](auto&... aggregators) {
          (remove_node_one(aggregators, node), ...);
        },
        aggregators_);
  }

 protected:
  /**
   * @brief Tuple storing instances of all aggregators.
   */
  Tuple aggregators_;

  // One-aggregator operation helpers
  /**
   * @brief Helper to update subtree data for one aggregator.
   *
   * For segment-supporting aggregators:
   * 1. Sets data from the current node
   * 2. Sets data from left subtree if it exists
   * 3. Sets data from right subtree if it exists
   *
   * For non-segment aggregators, simply calls update.
   *
   * @tparam Aggregator The type of the aggregator to update.
   * @tparam Node The type of the node to update from.
   * @param aggregator Reference to the aggregator to update.
   * @param node Pointer to the node whose data to use.
   */
  template <typename Aggregator, typename Node>
  static constexpr void update_one(Aggregator& aggregator, const Node* node) {
    if constexpr (Aggregator::support_segment) {
      set_node_one(aggregator, node);
      if (node->left) add_left_subtree_one(aggregator, node->left);
      if (node->right) add_right_subtree_one(aggregator, node->right);
    } else {
      aggregator.update(node);
    }
  }

  /**
   * @brief Helper to set subtree data from a single node for one aggregator.
   *
   * @tparam Aggregator The type of the aggregator to set.
   * @tparam Node The type of the source node.
   * @param aggregator Reference to the aggregator to set data for.
   * @param node Pointer to the node to set data from.
   */
  template <typename Aggregator, typename Node>
  static constexpr void set_node_one(Aggregator& aggregator, const Node* node) {
    aggregator.set_node(node);
  }

  /**
   * @brief Helper to set subtree data from a subtree for one aggregator.
   *
   * @tparam Aggregator The type of the aggregator to set.
   * @tparam Node The type of the source node.
   * @param aggregator Reference to the aggregator to set data for.
   * @param node Pointer to the root of the subtree to set data from.
   */
  template <typename Aggregator, typename Node>
  static constexpr void set_subtree_one(Aggregator& aggregator,
                                        const Node* node) {
    aggregator.set_subtree(node);
  }

  /**
   * @brief Helper to add subtree data from a single node for one aggregator.
   *
   * @tparam Aggregator The type of the aggregator to add to.
   * @tparam Node The type of the source node.
   * @param aggregator Reference to the aggregator to add data to.
   * @param node Pointer to the node to add data from.
   */
  template <typename Aggregator, typename Node>
  static constexpr void add_node_one(Aggregator& aggregator, const Node* node) {
    aggregator.add_node(node);
  }

  /**
   * @brief Helper to add subtree data from a subtree for one aggregator.
   *
   * @tparam Aggregator The type of the aggregator to add to.
   * @tparam Node The type of the source node.
   * @param aggregator Reference to the aggregator to add data to.
   * @param node Pointer to the root of the subtree to add data from.
   */
  template <typename Aggregator, typename Node>
  static constexpr void add_subtree_one(Aggregator& aggregator,
                                        const Node* node) {
    aggregator.add_subtree(node);
  }

  /**
   * @brief Helper to add left subtree data from a single node for one
   * aggregator. For order-independent aggregators, calls add_node instead.
   *
   * @tparam Aggregator The type of the aggregator to add to.
   * @tparam Node The type of the source node.
   * @param aggregator Reference to the aggregator to add data to.
   * @param node Pointer to the node to add data from.
   */
  template <typename Aggregator, typename Node>
  static constexpr void add_left_node_one(Aggregator& aggregator,
                                          const Node* node) {
    if constexpr (Aggregator::order_independent) {
      aggregator.add_node(node);
    } else {
      aggregator.add_left_node(node);
    }
  }

  /**
   * @brief Helper to add right subtree data from a single node for one
   * aggregator. For order-independent aggregators, calls add_node instead.
   *
   * @tparam Aggregator The type of the aggregator to add to.
   * @tparam Node The type of the source node.
   * @param aggregator Reference to the aggregator to add data to.
   * @param node Pointer to the node to add data from.
   */
  template <typename Aggregator, typename Node>
  static constexpr void add_right_node_one(Aggregator& aggregator,
                                           const Node* node) {
    if constexpr (Aggregator::order_independent) {
      aggregator.add_node(node);
    } else {
      aggregator.add_right_node(node);
    }
  }

  /**
   * @brief Helper to add left subtree data from a subtree for one aggregator.
   * For order-independent aggregators, calls add_subtree instead.
   *
   * @tparam Aggregator The type of the aggregator to add to.
   * @tparam Node The type of the source node.
   * @param aggregator Reference to the aggregator to add data to.
   * @param node Pointer to the root of the subtree to add data from.
   */
  template <typename Aggregator, typename Node>
  static constexpr void add_left_subtree_one(Aggregator& aggregator,
                                             const Node* node) {
    if constexpr (Aggregator::order_independent) {
      aggregator.add_subtree(node);
    } else {
      aggregator.add_left_subtree(node);
    }
  }

  /**
   * @brief Helper to add right subtree data from a subtree for one aggregator.
   * For order-independent aggregators, calls add_subtree instead.
   *
   * @tparam Aggregator The type of the aggregator to add to.
   * @tparam Node The type of the source node.
   * @param aggregator Reference to the aggregator to add data to.
   * @param node Pointer to the root of the subtree to add data from.
   */
  template <typename Aggregator, typename Node>
  static constexpr void add_right_subtree_one(Aggregator& aggregator,
                                              const Node* node) {
    if constexpr (Aggregator::order_independent) {
      aggregator.add_subtree(node);
    } else {
      aggregator.add_right_subtree(node);
    }
  }

  /**
   * @brief Helper to handle node insertion for one aggregator.
   *
   * @tparam Aggregator The type of the aggregator to insert into.
   * @tparam Node The type of the node to insert.
   * @param aggregator Reference to the aggregator to insert into.
   * @param node Pointer to the node being inserted.
   */
  template <typename Aggregator, typename Node>
  static constexpr void insert_node_one(Aggregator& aggregator,
                                        const Node* node) {
    aggregator.insert_node(node);
  }

  /**
   * @brief Helper to handle subtree insertion for one aggregator.
   *
   * @tparam Aggregator The type of the aggregator to insert into.
   * @tparam Node The type of the node to insert.
   * @param aggregator Reference to the aggregator to insert into.
   * @param node Pointer to the root of the subtree being inserted.
   */
  template <typename Aggregator, typename Node>
  static constexpr void insert_subtree_one(Aggregator& aggregator,
                                           const Node* node) {
    aggregator.insert_subtree(node);
  }

  /**
   * @brief Helper to handle node removal for one aggregator.
   *
   * @tparam Aggregator The type of the aggregator to remove from.
   * @tparam Node The type of the node to remove.
   * @param aggregator Reference to the aggregator to remove from.
   * @param node Pointer to the node being removed.
   */
  template <typename Aggregator, typename Node>
  static constexpr void remove_node_one(Aggregator& aggregator,
                                        const Node* node) {
    aggregator.remove_node(node);
  }
};

}  // namespace base
}  // namespace bst
