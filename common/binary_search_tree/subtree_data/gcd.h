#pragma once

#include "common/numeric/utils/gcd.h"

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the GCD of values in a BST subtree.
 *
 * This class extends another subtree data class to add GCD functionality.
 * It maintains the GCD of the node's data and the GCDs of its left and right
 * subtrees.
 *
 * @tparam GCDType The type used for the GCD calculation.
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename GCDType, typename BaseData>
class GCD : public BaseData {
 public:
  using Base = BaseData;
  using Self = GCD<GCDType, BaseData>;

  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  /**
   * @brief The GCD of all values in the subtree.
   *
   * Includes the node's data and the GCDs from both left and right subtrees.
   */
  GCDType gcd_value;

 public:
  /**
   * @brief Updates the GCD of values in the subtree.
   *
   * Calculates the GCD of the node's data and the GCDs from
   * the left and right subtrees.
   *
   * @param node The node to update.
   */
  template <typename Node>
  constexpr void update(Node* node) {
    Base::update(node);
    gcd_value = GCDType(node->data);
    if (node->l) gcd_value = ::GCD(gcd_value, node->l->subtree_data.gcd_value);
    if (node->r) gcd_value = ::GCD(gcd_value, node->r->subtree_data.gcd_value);
  }
};

}  // namespace subtree_data
}  // namespace bst
