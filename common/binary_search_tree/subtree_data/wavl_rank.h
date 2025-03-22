#pragma once

#include "common/base.h"

#include <algorithm>

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the rank of a node in a WAVL tree.
 *
 * This class extends another subtree data class to add rank tracking
 * for WAVL (Weak AVL) tree operations. The rank is used to maintain
 * the weak AVL property of the tree.
 *
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename BaseData>
class WAVLRank : public BaseData {
 public:
  using Base = BaseData;
  using Self = WAVLRank<BaseData>;

 public:
  /**
   * @brief The rank of the node in the WAVL tree.
   *
   * Used to maintain the weak AVL property. Initialized to 0.
   */
  int rank = 0;

 public:
  /**
   * @brief Resets the WAVL tree information.
   *
   * Sets the node rank to 0.
   */
  constexpr void bti_reset() {
    Base::bti_reset();
    rank = 0;
  }

  /**
   * @brief Copies WAVL tree information from another node.
   *
   * @param node The source node to copy from.
   */
  template <typename Node>
  constexpr void bti_copy(const Node* node) {
    assert(node);
    Base::bti_copy(node);
    rank = node->subtree_data.rank;
  }

  /**
   * @brief Swaps WAVL tree information with another node.
   *
   * @param node The node to swap with.
   */
  template <typename Node>
  constexpr void bti_swap(Node* node) {
    assert(node);
    Base::bti_swap(node);
    std::swap(rank, node->subtree_data.rank);
  }
};

}  // namespace subtree_data
}  // namespace bst
