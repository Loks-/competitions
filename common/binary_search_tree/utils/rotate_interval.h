#pragma once

#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/utils/swap_intervals.h"

namespace bst {
namespace utils {

/**
 * @brief Rotates elements in an interval to the right by a specified shift.
 *
 * This function rotates the elements in the interval [first, last) to the right
 * by 'shift' positions. The rotation is performed by swapping two sub-intervals
 * within the specified range.
 *
 * The operation is equivalent to moving the last 'shift' elements to the
 * beginning of the interval, shifting all other elements to the right.
 *
 * Trivial cases are handled gracefully:
 * - If last <= first, the function returns without changes
 * - If shift is 0 or a multiple of the interval size, no rotation occurs
 *
 * @tparam TTree The tree type that provides the required operations.
 * @tparam TNode The node type.
 * @param root The root of the tree to modify (passed by reference).
 * @param first The beginning of the interval (inclusive).
 * @param last The end of the interval (exclusive).
 * @param shift The number of positions to rotate right.
 */
template <typename TTree, typename TNode>
constexpr void rotate_interval_right(TNode*& root, size_t first, size_t last,
                                     size_t shift) {
  // Handle trivial cases
  if (last <= first) return;

  shift %= (last - first);
  if (shift != 0) utils::swap_intervals<TTree>(root, first, last - shift, last);
}

/**
 * @brief Rotates elements in an interval to the left by a specified shift.
 *
 * This function rotates the elements in the interval [first, last) to the left
 * by 'shift' positions. The rotation is performed by swapping two sub-intervals
 * within the specified range.
 *
 * The operation is equivalent to moving the first 'shift' elements to the
 * end of the interval, shifting all other elements to the left.
 *
 * Trivial cases are handled gracefully:
 * - If last <= first, the function returns without changes
 * - If shift is 0 or a multiple of the interval size, no rotation occurs
 *
 * @tparam TTree The tree type that provides the required operations.
 * @tparam TNode The node type.
 * @param root The root of the tree to modify (passed by reference).
 * @param first The beginning of the interval (inclusive).
 * @param last The end of the interval (exclusive).
 * @param shift The number of positions to rotate left.
 */
template <typename TTree, typename TNode>
constexpr void rotate_interval_left(TNode*& root, size_t first, size_t last,
                                    size_t shift) {
  // Handle trivial cases
  if (last <= first) return;

  shift %= (last - first);
  if (shift != 0)
    utils::swap_intervals<TTree>(root, first, first + shift, last);
}

/**
 * @brief Rotates all elements in the tree to the right by a specified shift.
 *
 * This function rotates all elements in the entire tree to the right by
 * 'shift' positions. It's equivalent to calling rotate_interval_right on
 * the entire tree range [0, size).
 *
 * The operation is useful for implementing circular shifts or rotations
 * on the entire tree structure.
 *
 * @tparam TTree The tree type that provides the required operations.
 * @tparam TNode The node type.
 * @param root The root of the tree to modify (passed by reference).
 * @param shift The number of positions to rotate right.
 */
template <typename TTree, typename TNode>
constexpr void rotate_tree_right(TNode*& root, size_t shift) {
  if (root) {
    const size_t tree_size = bst::subtree_data::size(root);
    rotate_interval_right<TTree>(root, 0, tree_size, shift);
  }
}

/**
 * @brief Rotates all elements in the tree to the left by a specified shift.
 *
 * This function rotates all elements in the entire tree to the left by
 * 'shift' positions. It's equivalent to calling rotate_interval_left on
 * the entire tree range [0, size).
 *
 * The operation is useful for implementing circular shifts or rotations
 * on the entire tree structure.
 *
 * @tparam TTree The tree type that provides the required operations.
 * @tparam TNode The node type.
 * @param root The root of the tree to modify (passed by reference).
 * @param shift The number of positions to rotate left.
 */
template <typename TTree, typename TNode>
constexpr void rotate_tree_left(TNode*& root, size_t shift) {
  if (root) {
    const size_t tree_size = bst::subtree_data::size(root);
    rotate_interval_left<TTree>(root, 0, tree_size, shift);
  }
}

}  // namespace utils
}  // namespace bst
