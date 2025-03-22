#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/none.h"

namespace bst {
namespace subtree_data {
namespace segment {

/**
 * @brief A base class for BST segment data that stores nothing.
 *
 * This class extends the base None class to add segment-specific functionality.
 * It provides empty implementations of all required segment methods and
 * maintains the same "no data" properties as the base class.
 */
class None : public bst::subtree_data::None {
 public:
  using Base = bst::subtree_data::None;
  using Self = None;

 public:
  /**
   * @brief Sets the node segment data.
   *
   * Empty implementation as this class stores no data.
   *
   * @param node The node to set data for.
   */
  template <typename Node>
  constexpr void set_node([[maybe_unused]] const Node* node) {
    assert(node);
  }

  /**
   * @brief Sets the subtree segment data.
   *
   * Empty implementation as this class stores no data.
   *
   * @param node The node to set data for.
   */
  template <typename Node>
  constexpr void set_subtree([[maybe_unused]] const Node* node) {
    assert(node);
  }

  /**
   * @brief Adds node segment data.
   *
   * Empty implementation as this class stores no data.
   *
   * @param node The node to add data from.
   */
  template <typename Node>
  constexpr void add_node(const Node*) {}

  /**
   * @brief Adds subtree segment data.
   *
   * Empty implementation as this class stores no data.
   *
   * @param node The node to add data from.
   */
  template <typename Node>
  constexpr void add_subtree(const Node*) {}
};

}  // namespace segment
}  // namespace subtree_data
}  // namespace bst
