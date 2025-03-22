#pragma once

#include "common/hash.h"

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the height of a node in a Treap.
 *
 * This class extends another subtree data class to add Treap height tracking.
 * The height is determined by a hash function applied to the node's raw index,
 * which helps maintain the Treap's heap property.
 *
 * @tparam HeightType The type used for the height value.
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename HeightType, typename BaseData>
class TreapHeight : public BaseData {
 public:
  using Base = BaseData;
  using Self = TreapHeight<HeightType, BaseData>;

 public:
  /**
   * @brief The height of the node in the Treap.
   *
   * This value is used to maintain the heap property of the Treap.
   * It is calculated using a hash function applied to the node's raw index.
   */
  HeightType treap_height;

 public:
  /**
   * @brief Initializes the Treap height for a new node.
   *
   * Calculates the height using a hash function applied to the node's raw
   * index. This ensures a good distribution of heights for the Treap's heap
   * property.
   *
   * @param raw_index The raw index of the node.
   */
  constexpr void clear_create(unsigned raw_index) {
    Base::clear_create(raw_index);
    treap_height = HeightType(nhash::HValue<nhash::PolicySafe>(raw_index));
  }
};

}  // namespace subtree_data
}  // namespace bst
