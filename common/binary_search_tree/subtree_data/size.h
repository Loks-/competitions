#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/none.h"

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the size of a BST subtree.
 *
 * This class extends the None class to add subtree size tracking.
 * It maintains the total number of nodes in the subtree, including
 * the current node and all its descendants.
 */
class Size : public None {
 public:
  using Base = None;
  using Self = Size;

  static constexpr bool is_none = false;
  static constexpr bool has_size = true;

 public:
  /**
   * @brief The total number of nodes in the subtree.
   *
   * Includes the current node and all its descendants.
   * Initialized to 0.
   */
  size_t size = 0;

 public:
  /**
   * @brief Updates the size of the subtree.
   *
   * Calculates the total size as 1 (current node) plus the sizes
   * of the left and right subtrees.
   *
   * @param node The node to update.
   */
  template <typename Node>
  constexpr void update(Node* node) {
    size = 1 + (node->l ? node->l->subtree_data.size : 0) +
           (node->r ? node->r->subtree_data.size : 0);
  }
};

}  // namespace subtree_data
}  // namespace bst
