#pragma once

#include "common/base.h"

namespace bst {
namespace utils {

/**
 * @brief Swaps two intervals in a binary search tree by index.
 *
 * This function swaps the intervals [first, middle) and [middle, last) in the
 * tree. It works by splitting the tree into four parts and then rejoining them
 * in the swapped order.
 *
 * The operation splits the tree at three points:
 * - At 'first' to separate the leftmost part
 * - At 'middle' to separate the two intervals
 * - At 'last' to separate the rightmost part
 *
 * Then rejoins them as: (left_part + second_interval) + (first_interval +
 * right_part)
 *
 * Trivial cases are handled gracefully:
 * - If middle <= first or middle >= last, the function returns without changes
 * - Invalid intervals result in no operation
 *
 * @tparam TTree The tree type that provides split_at and join methods.
 * @tparam TNode The node type.
 * @param root The root of the tree to modify (passed by reference).
 * @param first The beginning of the first interval (inclusive).
 * @param middle The end of the first interval and beginning of the second
 * interval.
 * @param last The end of the second interval (exclusive).
 */
template <typename TTree, typename TNode>
constexpr void swap_intervals(TNode*& root, size_t first, size_t middle,
                              size_t last) {
  static_assert(!TTree::has_key, "has_key should be false");

  // Handle trivial cases
  if ((middle <= first) || (middle >= last)) return;

  TNode *left_part, *first_interval, *second_interval, *right_part;

  // Split the tree at 'middle' to separate the two intervals
  TTree::split_at(root, middle, first_interval, second_interval);

  // Split the left part at 'first' to get the leftmost section
  TTree::split_at(first_interval, first, left_part, first_interval);

  // Split the right part at 'last - middle' to get the rightmost section
  TTree::split_at(second_interval, last - middle, second_interval, right_part);

  // Rejoin in swapped order: (left_part + second_interval) + (first_interval +
  // right_part)
  root = TTree::join(TTree::join(left_part, second_interval),
                     TTree::join(first_interval, right_part));
}

}  // namespace utils
}  // namespace bst
