#pragma once

#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace subtree_data {
namespace segment {

/**
 * @brief A class that maintains the size of a BST segment.
 *
 * This class extends the base Size class to add segment-specific functionality.
 * It provides methods to set and add sizes for both individual nodes and
 * entire subtrees.
 */
class Size : public bst::subtree_data::Size {
 public:
  using Base = bst::subtree_data::Size;
  using Self = Size;

 public:
  /**
   * @brief Sets the size for a single node.
   *
   * Sets the size to 1, representing a single node.
   *
   * @param node The node to set size for.
   */
  template <typename Node>
  constexpr void set_node([[maybe_unused]] const Node* node) {
    assert(node);
    Base::size = 1;
  }

  /**
   * @brief Sets the size for a subtree.
   *
   * Sets the size to match the size of the given subtree.
   *
   * @param node The root of the subtree to get size from.
   */
  template <typename Node>
  constexpr void set_subtree(const Node* node) {
    assert(node);
    Base::size = node->subtree_data.size;
  }

  /**
   * @brief Adds the size of a single node.
   *
   * Adds 1 to the current size if the node exists.
   *
   * @param node The node to add size from.
   */
  template <typename Node>
  constexpr void add_node(const Node* node) {
    if (node) {
      Base::size += 1;
    }
  }

  /**
   * @brief Adds the size of a subtree.
   *
   * Adds the size of the given subtree to the current size if the node exists.
   *
   * @param node The root of the subtree to add size from.
   */
  template <typename Node>
  constexpr void add_subtree(const Node* node) {
    if (node) {
      Base::size += node->subtree_data.size;
    }
  }
};

}  // namespace segment
}  // namespace subtree_data
}  // namespace bst
